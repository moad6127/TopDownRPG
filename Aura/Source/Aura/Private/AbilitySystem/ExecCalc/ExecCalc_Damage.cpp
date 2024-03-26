// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/ExecCalc/ExecCalc_Damage.h"

#include "AbilitySystemComponent.h"
#include "TopDownRPGGameplayTags.h"
#include "AbilitySystem/TopDownRPGAttributeSet.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "AbilitySystem/TopDownRPGAbilitySystemLibrary.h"
#include "Interaction/CombatInterface.h"
#include "TopDownRPGAbilityTypes.h"

// 블루프린트에 노출시키지 않는 원시적인 Struct이다.
struct TopDownRPGDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration);
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage);
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance);
	TopDownRPGDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTopDownRPGAttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTopDownRPGAttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTopDownRPGAttributeSet, BlockChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTopDownRPGAttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTopDownRPGAttributeSet, CriticalHitDamage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UTopDownRPGAttributeSet, CriticalHitResistance, Target, false);
	}
};

static const TopDownRPGDamageStatics& DamageStatics()
{
	static TopDownRPGDamageStatics DStatics;
	return DStatics;
}

UExecCalc_Damage::UExecCalc_Damage()
{
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
	RelevantAttributesToCapture.Add(DamageStatics().BlockChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitDamageDef);
	RelevantAttributesToCapture.Add(DamageStatics().CriticalHitResistanceDef);
}

void UExecCalc_Damage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	const UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();

	AActor* SourceAvatar = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetAvatar = TargetASC ? TargetASC->GetAvatarActor() : nullptr;
	ICombatInterface* SourceCombatInterface = Cast<ICombatInterface>(SourceAvatar);
	ICombatInterface* TargetCombatInterface = Cast<ICombatInterface>(TargetAvatar);

	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	const FGameplayTagContainer* SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	const FGameplayTagContainer* TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	FAggregatorEvaluateParameters EvaluationParameters;
	EvaluationParameters.SourceTags = SourceTags;
	EvaluationParameters.TargetTags = TargetTags;
	
	// Get Damage set by caller Magnitude;
	float Damage = 0.f;
	for (const auto& Pair : FTopDownRPGGameplayTags::Get().DamageTypesToResistances)
	{
		const float DamageTypeValue = Spec.GetSetByCallerMagnitude(Pair.Key);
		Damage += DamageTypeValue;
	}
	//Blockchance를 캡처한후 Block됬는지를 판단한다
	float TargetBlockChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().BlockChanceDef, EvaluationParameters, TargetBlockChance);
	TargetBlockChance = FMath::Max<float>(TargetBlockChance, 0.f);

	bool bBlocking = FMath::RandRange(1, 100) < TargetBlockChance;

	FGameplayEffectContextHandle EffectContextHandle = Spec.GetContext();
	UTopDownRPGAbilitySystemLibrary::SetIsBlockedHit(EffectContextHandle, bBlocking);

	//블록되면 Damage를 /2로 만든다
	Damage = bBlocking ? Damage / 2.f : Damage;

	// Target의 Armor과 Source의 ArmorPenetration를 캡처한다.
	float TargetArmor = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorDef, EvaluationParameters, TargetArmor);
	TargetArmor = FMath::Max<float>(TargetArmor, 0.f);

	float SourceArmorPenetration = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().ArmorPenetrationDef, EvaluationParameters, SourceArmorPenetration);
	SourceArmorPenetration = FMath::Max<float>(SourceArmorPenetration, 0.f);

	const UCharacterClassInfo* CharacterClassInfo = UTopDownRPGAbilitySystemLibrary::GetCharacterClassInfo(SourceAvatar);
	const FRealCurve* ArmorPenetrationCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("ArmorPenetration"), FString());
	const float ArmorPenetrationCoefficients = ArmorPenetrationCurve->Eval(SourceCombatInterface->GetPlayerLevel());
	
	//Source의 방어관통력을 Target의 Armor에 적용시킨다(백분율로 적용시킴)
	const float EffectiveArmor = TargetArmor * (100 - SourceArmorPenetration * ArmorPenetrationCoefficients) / 100.f;

	const FRealCurve* EffectiveArmorCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("EffectiveArmor"), FString());
	const float EffectiveArmorCoefficients = EffectiveArmorCurve->Eval(TargetCombatInterface->GetPlayerLevel());
	// 적용된Target의 Armor에 따라서 Damage를 감소시킨다.
	Damage *= (100 - EffectiveArmor * EffectiveArmorCoefficients) / 100.f;

	//Critical선별하기
	float SourceCriticalChance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitChanceDef, EvaluationParameters, SourceCriticalChance);
	SourceCriticalChance = FMath::Max<float>(SourceCriticalChance, 0.f);

	//Critical추가 Damage
	float SourceCriticalDamage = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitDamageDef, EvaluationParameters, SourceCriticalDamage);
	SourceCriticalDamage = FMath::Max<float>(SourceCriticalDamage, 0.f);

	//CriticalResistance발동시 Damage는 원상복구
	float TargetCriticalResistance = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(DamageStatics().CriticalHitResistanceDef, EvaluationParameters, TargetCriticalResistance);
	TargetCriticalResistance = FMath::Max<float>(TargetCriticalResistance, 0.f);

	const FRealCurve* CriticalResistanceCurve = CharacterClassInfo->DamageCalculationCoefficients->FindCurve(FName("CriticalHitResistance"), FString());
	const float CriticalHitResistanceCoefficients = CriticalResistanceCurve->Eval(TargetCombatInterface->GetPlayerLevel());

	const float EffectiveCriticalHitChance = SourceCriticalChance - TargetCriticalResistance * CriticalHitResistanceCoefficients;
	const bool bCritical = FMath::RandRange(1, 100) < EffectiveCriticalHitChance;

	UTopDownRPGAbilitySystemLibrary::SetIsCriticalHit(EffectContextHandle, bCritical);

	Damage = bCritical ? Damage * 2.f + SourceCriticalDamage : Damage;

	const FGameplayModifierEvaluatedData EvaluatedData(
		UTopDownRPGAttributeSet::GetIncomingDamageAttribute(),
		EGameplayModOp::Additive,
		Damage);
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
