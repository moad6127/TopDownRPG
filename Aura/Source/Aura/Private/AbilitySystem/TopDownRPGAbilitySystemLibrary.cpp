// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/TopDownRPGAbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/TopDownRPGHUD.h"
#include "Player/TopDownRPGPlayerState.h"
#include "Game/TopDownRPGGameModeBase.h"
#include "AbilitySystemComponent.h"
#include "TopDownRPGAbilityTypes.h"
#include "Interaction/CombatInterface.h"
#include "UI/WidgetController/TopDownRPGWidgetController.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "TopDownRPGGameplayTags.h"
#include "Game/LoadScreenSaveGame.h"

bool UTopDownRPGAbilitySystemLibrary::MakeWidgetControllerParams(const UObject* WorldContextObject, FWidgetControllerParams& OutWCParams, ATopDownRPGHUD*& OutTopDownRPGHUD)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		OutTopDownRPGHUD = Cast<ATopDownRPGHUD>(PC->GetHUD());
		if (OutTopDownRPGHUD)
		{
			ATopDownRPGPlayerState* PS = PC->GetPlayerState<ATopDownRPGPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			
			OutWCParams.AttributeSet = AS;
			OutWCParams.AbilitySystemComponent = ASC;
			OutWCParams.PlayerController = PC;
			OutWCParams.PlayerState = PS;
			return true;
		}
	}
	return false;
}

UOverlayWidgetController* UTopDownRPGAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	ATopDownRPGHUD* TopDownRPGHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, TopDownRPGHUD))
	{
		return TopDownRPGHUD->GetOverlayWidgetController(WCParams);
	}
	return nullptr;
}

UAttributeMenuWidgetController* UTopDownRPGAbilitySystemLibrary::GetAttributeMenuController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	ATopDownRPGHUD* TopDownRPGHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, TopDownRPGHUD))
	{
		return TopDownRPGHUD->GetAttributeMenuController(WCParams);
	}
	return nullptr;

}

USpellMenuWidgetController* UTopDownRPGAbilitySystemLibrary::GetSpellMenuController(const UObject* WorldContextObject)
{
	FWidgetControllerParams WCParams;
	ATopDownRPGHUD* TopDownRPGHUD = nullptr;
	if (MakeWidgetControllerParams(WorldContextObject, WCParams, TopDownRPGHUD))
	{
		return TopDownRPGHUD->GetSpellMenuController(WCParams);
	}
	return nullptr;
}

void UTopDownRPGAbilitySystemLibrary::InitializeDefaultsAttributes(const UObject* WorldContextObject, ECharacterClass CharacterClass, float Level, UAbilitySystemComponent* ASC)
{
	AActor* AvatarActor = ASC->GetAvatarActor();

	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	FCharacterClassDefaultInfo ClassDefaultsInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);

	FGameplayEffectContextHandle PrimaryAttributeContextHandle = ASC->MakeEffectContext();
	PrimaryAttributeContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle PrimaryAttributeSpecHandle = ASC->MakeOutgoingSpec(
		ClassDefaultsInfo.PrimaryAttributes,
		Level,
		PrimaryAttributeContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*PrimaryAttributeSpecHandle.Data.Get());

	FGameplayEffectContextHandle SecondaryAttributeContextHandle = ASC->MakeEffectContext();
	SecondaryAttributeContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributeSpecHandle = ASC->MakeOutgoingSpec(
		CharacterClassInfo->SecondaryAttributes,
		Level,
		SecondaryAttributeContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributeSpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalAttributeContextHandle = ASC->MakeEffectContext();
	VitalAttributeContextHandle.AddSourceObject(AvatarActor);
	const FGameplayEffectSpecHandle VitalAttributeSpecHandle = ASC->MakeOutgoingSpec(
		CharacterClassInfo->VitalAttributes,
		Level,
		VitalAttributeContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributeSpecHandle.Data.Get());
}

void UTopDownRPGAbilitySystemLibrary::InitializeDefaultsAttributesFromSaveData(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ULoadScreenSaveGame* SaveGame)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr)
	{
		return;
	}
	const FTopDownRPGGameplayTags& GameplayTag = FTopDownRPGGameplayTags::Get();
	const AActor* SourceAvatarActor = ASC->GetAvatarActor();


	FGameplayEffectContextHandle EffectContextHandle = ASC->MakeEffectContext();
	EffectContextHandle.AddSourceObject(SourceAvatarActor);

	const FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(CharacterClassInfo->PrimaryAttributes_SetByCaller, 1.f, EffectContextHandle);


	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle,GameplayTag.Attributes_Primary_Strength, SaveGame->Strength);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTag.Attributes_Primary_Intelligence, SaveGame->Intelligence);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTag.Attributes_Primary_Resilience, SaveGame->Resilience);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTag.Attributes_Primary_Vigor, SaveGame->Vigor);

	ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);


	FGameplayEffectContextHandle SecondaryAttributeContextHandle = ASC->MakeEffectContext();
	SecondaryAttributeContextHandle.AddSourceObject(SourceAvatarActor);
	const FGameplayEffectSpecHandle SecondaryAttributeSpecHandle = ASC->MakeOutgoingSpec(
		CharacterClassInfo->SecondaryAttributes_Infinite,
		1.f,
		SecondaryAttributeContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*SecondaryAttributeSpecHandle.Data.Get());

	FGameplayEffectContextHandle VitalAttributeContextHandle = ASC->MakeEffectContext();
	VitalAttributeContextHandle.AddSourceObject(SourceAvatarActor);
	const FGameplayEffectSpecHandle VitalAttributeSpecHandle = ASC->MakeOutgoingSpec(
		CharacterClassInfo->VitalAttributes,
		1.f,
		VitalAttributeContextHandle);
	ASC->ApplyGameplayEffectSpecToSelf(*VitalAttributeSpecHandle.Data.Get());
}

void UTopDownRPGAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC, ECharacterClass CharacterClass)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr)
	{
		return;
	}
	for (TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
	}
	const FCharacterClassDefaultInfo& DefaultInfo = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	for (TSubclassOf<UGameplayAbility> AbilityClass : DefaultInfo.StartupAbilities)
	{
		if (ASC->GetAvatarActor()->Implements<UCombatInterface>())
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass,ICombatInterface::Execute_GetPlayerLevel(ASC->GetAvatarActor()));
			ASC->GiveAbility(AbilitySpec);
		}
	}
}

UCharacterClassInfo* UTopDownRPGAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	const ATopDownRPGGameModeBase* TopDownRPGGameMode = Cast<ATopDownRPGGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (TopDownRPGGameMode == nullptr)
	{
		return nullptr;
	}
	return TopDownRPGGameMode->CharacterClassInfo;
}

UAbilityInfo* UTopDownRPGAbilitySystemLibrary::GetAbilityInfo(const UObject* WorldContextObject)
{
	const ATopDownRPGGameModeBase* TopDownRPGGameMode = Cast<ATopDownRPGGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (TopDownRPGGameMode == nullptr)
	{
		return nullptr;
	}
	return TopDownRPGGameMode->AbilityInfo;
}

bool UTopDownRPGAbilitySystemLibrary::IsBlockedHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FTopDownRPGGameplayEffectContext* TopDownRPGEffectContext = static_cast<const FTopDownRPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return TopDownRPGEffectContext->IsBlockedHit();
	}
	return false;
}

bool UTopDownRPGAbilitySystemLibrary::IsCriticalHit(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FTopDownRPGGameplayEffectContext* TopDownRPGEffectContext = static_cast<const FTopDownRPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return TopDownRPGEffectContext->IsCriticalHit();
	}
	return false;
}

bool UTopDownRPGAbilitySystemLibrary::IsSuccessfulDebuff(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FTopDownRPGGameplayEffectContext* TopDownRPGEffectContext = static_cast<const FTopDownRPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return TopDownRPGEffectContext->IsSuccessfulDebuff();
	}
	return false;
}

float UTopDownRPGAbilitySystemLibrary::GetDebuffDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FTopDownRPGGameplayEffectContext* TopDownRPGEffectContext = static_cast<const FTopDownRPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return TopDownRPGEffectContext->GetDebuffDamage();
	}
	return 0.0f;
}

float UTopDownRPGAbilitySystemLibrary::GetDebuffDuration(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FTopDownRPGGameplayEffectContext* TopDownRPGEffectContext = static_cast<const FTopDownRPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return TopDownRPGEffectContext->GetDebuffDuration();
	}
	return 0.0f;
}

float UTopDownRPGAbilitySystemLibrary::GetDebuffFrequency(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FTopDownRPGGameplayEffectContext* TopDownRPGEffectContext = static_cast<const FTopDownRPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return TopDownRPGEffectContext->GetDebuffFrequency();
	}
	return 0.0f;
}

FGameplayTag UTopDownRPGAbilitySystemLibrary::GetDamageType(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FTopDownRPGGameplayEffectContext* TopDownRPGEffectContext = static_cast<const FTopDownRPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		if (TopDownRPGEffectContext->GetDamageType().IsValid())
		{
			return *TopDownRPGEffectContext->GetDamageType();
		}
	}
	return FGameplayTag();
}

FVector UTopDownRPGAbilitySystemLibrary::GetDeathImpulse(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FTopDownRPGGameplayEffectContext* TopDownRPGEffectContext = static_cast<const FTopDownRPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return TopDownRPGEffectContext->GetDeathImpulse();
	}
	return FVector::ZeroVector;
}

FVector UTopDownRPGAbilitySystemLibrary::GetKnockbackForce(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FTopDownRPGGameplayEffectContext* TopDownRPGEffectContext = static_cast<const FTopDownRPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return TopDownRPGEffectContext->GetKnockBackForce();
	}
	return FVector::ZeroVector;
}

bool UTopDownRPGAbilitySystemLibrary::IsRadialDamage(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FTopDownRPGGameplayEffectContext* TopDownRPGEffectContext = static_cast<const FTopDownRPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return TopDownRPGEffectContext->IsRadialDamage();
	}
	return false;
}

float UTopDownRPGAbilitySystemLibrary::GetRadialDamageInnerRadius(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FTopDownRPGGameplayEffectContext* TopDownRPGEffectContext = static_cast<const FTopDownRPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return TopDownRPGEffectContext->GetRadialDamageInnerRadius();
	}
	return 0.0f;
}

float UTopDownRPGAbilitySystemLibrary::GetRadialDamageOuterRadius(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FTopDownRPGGameplayEffectContext* TopDownRPGEffectContext = static_cast<const FTopDownRPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return TopDownRPGEffectContext->GetRadialDamageOuterRadius();
	}
	return 0.0f;
}

FVector UTopDownRPGAbilitySystemLibrary::GetRadialDamageOrigin(const FGameplayEffectContextHandle& EffectContextHandle)
{
	if (const FTopDownRPGGameplayEffectContext* TopDownRPGEffectContext = static_cast<const FTopDownRPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		return TopDownRPGEffectContext->GetRadialDamageOrigin();
	}
	return FVector::ZeroVector;
}

void UTopDownRPGAbilitySystemLibrary::SetIsBlockedHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsBlockedHit)
{
	if (FTopDownRPGGameplayEffectContext* TopDownRPGEffectContext = static_cast<FTopDownRPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		TopDownRPGEffectContext->SetIsBlockedHit(bInIsBlockedHit);
	}
}

void UTopDownRPGAbilitySystemLibrary::SetIsCriticalHit(FGameplayEffectContextHandle& EffectContextHandle, bool bInIsCriticalHit)
{
	if (FTopDownRPGGameplayEffectContext* TopDownRPGEffectContext = static_cast<FTopDownRPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		TopDownRPGEffectContext->SetIsCriticalHit(bInIsCriticalHit);
	}
}

void UTopDownRPGAbilitySystemLibrary::SetIsSuccessfulDebuff(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, bool bInIsSuccessfulDebuff)
{
	if (FTopDownRPGGameplayEffectContext* TopDownRPGEffectContext = static_cast<FTopDownRPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		TopDownRPGEffectContext->SetIsSuccessfulDebuff(bInIsSuccessfulDebuff);
	}
}

void UTopDownRPGAbilitySystemLibrary::SetDebuffDamage(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, float InDamage)
{
	if (FTopDownRPGGameplayEffectContext* TopDownRPGEffectContext = static_cast<FTopDownRPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		TopDownRPGEffectContext->SetDebuffDamage(InDamage);
	}
}
void UTopDownRPGAbilitySystemLibrary::SetDebuffDuration(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, float InDebuffDuration)
{
	if (FTopDownRPGGameplayEffectContext* TopDownRPGEffectContext = static_cast<FTopDownRPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		TopDownRPGEffectContext->SetDebuffDuration(InDebuffDuration);
	}
}

void UTopDownRPGAbilitySystemLibrary::SetDebuffFrequency(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, float InDebuffFrequency)
{
	if (FTopDownRPGGameplayEffectContext* TopDownRPGEffectContext = static_cast<FTopDownRPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		TopDownRPGEffectContext->SetDebuffFrequency(InDebuffFrequency);
	}
}

void UTopDownRPGAbilitySystemLibrary::SetDamageType(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, const FGameplayTag& InDamageType)
{
	if (FTopDownRPGGameplayEffectContext* TopDownRPGEffectContext = static_cast<FTopDownRPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		const TSharedPtr<FGameplayTag> DamageType = MakeShared<FGameplayTag>(InDamageType);
		TopDownRPGEffectContext->SetDamageType(DamageType);
	}
}

void UTopDownRPGAbilitySystemLibrary::SetDeathImpulse(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, const FVector& InImpulse)
{
	if (FTopDownRPGGameplayEffectContext* TopDownRPGEffectContext = static_cast<FTopDownRPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		TopDownRPGEffectContext->SetDeathImpulse(InImpulse);
	}
}

void UTopDownRPGAbilitySystemLibrary::SetKnockbackForce(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, const FVector& InForce)
{
	if (FTopDownRPGGameplayEffectContext* TopDownRPGEffectContext = static_cast<FTopDownRPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		TopDownRPGEffectContext->SetKnockbackForce(InForce);
	}
}

void UTopDownRPGAbilitySystemLibrary::SetIsRadialDamage(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, bool bInIsRadialDamage)
{
	if (FTopDownRPGGameplayEffectContext* TopDownRPGEffectContext = static_cast<FTopDownRPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		TopDownRPGEffectContext->SetIsRadialDamage(bInIsRadialDamage);
	}
}

void UTopDownRPGAbilitySystemLibrary::SetRadialDamageInnerRadius(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, float InInnerRadius)
{
	if (FTopDownRPGGameplayEffectContext* TopDownRPGEffectContext = static_cast<FTopDownRPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		TopDownRPGEffectContext->SetRadialDamageInnerRadius(InInnerRadius);
	}
}

void UTopDownRPGAbilitySystemLibrary::SetRadialDamageOuterRadius(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, float InOuterRadius)
{
	if (FTopDownRPGGameplayEffectContext* TopDownRPGEffectContext = static_cast<FTopDownRPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		TopDownRPGEffectContext->SetRadialDamageOuterRadius(InOuterRadius);
	}
}

void UTopDownRPGAbilitySystemLibrary::SetRadialDamageOrigin(UPARAM(ref)FGameplayEffectContextHandle& EffectContextHandle, const FVector& InOrigin)
{
	if (FTopDownRPGGameplayEffectContext* TopDownRPGEffectContext = static_cast<FTopDownRPGGameplayEffectContext*>(EffectContextHandle.Get()))
	{
		TopDownRPGEffectContext->SetRadialDamageOrigin(InOrigin);
	}
}


void UTopDownRPGAbilitySystemLibrary::GetLivePlayerWithRadius(const UObject* WorldContextObject, TArray<AActor*>& OutOverlappingActors, const TArray<AActor*>& ActorToIgnore, float Radious, const FVector& SphereOrigin)
{
	FCollisionQueryParams SphereParams;
	SphereParams.AddIgnoredActors(ActorToIgnore);

	TArray<FOverlapResult> Overlaps;
	if (const UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		World->OverlapMultiByObjectType(Overlaps, SphereOrigin, FQuat::Identity, FCollisionObjectQueryParams(FCollisionObjectQueryParams::InitType::AllDynamicObjects), FCollisionShape::MakeSphere(Radious), SphereParams);
		for (FOverlapResult& Overlap : Overlaps)
		{
			if (Overlap.GetActor()->Implements<UCombatInterface>() && !ICombatInterface::Execute_IsDead(Overlap.GetActor()))
			{
				OutOverlappingActors.AddUnique(ICombatInterface::Execute_GetAvatar(Overlap.GetActor()));
			}
		}
	}
}

void UTopDownRPGAbilitySystemLibrary::GetClosestTargets(int32 MaxTarget, const TArray<AActor*>& Actors, TArray<AActor*>& OutClosestTargets, const FVector& Origin)
{
	if (Actors.Num() <= MaxTarget)
	{
		OutClosestTargets = Actors;
		return;
	}
	TArray<AActor*> ActorToCheck = Actors;
	Algo::Sort(ActorToCheck, [Origin](const AActor* Actor1, const AActor* Actor2)
		{
			const float Distance1 = FVector::DistSquared(Actor1->GetActorLocation(), Origin);
			const float Distance2 = FVector::DistSquared(Actor2->GetActorLocation(), Origin);

			return Distance1 < Distance2;
		});

	for (int32 i = 0; i < MaxTarget; i++)
	{
		OutClosestTargets.Add(ActorToCheck[i]);
	}
}

bool UTopDownRPGAbilitySystemLibrary::IsNotFriend(AActor* FirstActor, AActor* SecondActor)
{
	const bool bBothArePlayer = FirstActor->ActorHasTag(FName("Player")) && SecondActor->ActorHasTag(FName("Player"));
	const bool bBothAreEnemy = FirstActor->ActorHasTag(FName("Enemy")) && SecondActor->ActorHasTag(FName("Enemy"));
	const bool bFriend = bBothArePlayer || bBothAreEnemy;

	return !bFriend;
}

FGameplayEffectContextHandle UTopDownRPGAbilitySystemLibrary::ApplyDamageEffect(const FDamageEffectParams& DamageEffectParams)
{
	const FTopDownRPGGameplayTags& GameplayTag = FTopDownRPGGameplayTags::Get();
	const AActor* SourceAvatarActor = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();


	FGameplayEffectContextHandle EffectContextHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeEffectContext();

	EffectContextHandle.AddSourceObject(SourceAvatarActor);
	SetDeathImpulse(EffectContextHandle, DamageEffectParams.DeathImpulse);
	SetKnockbackForce(EffectContextHandle, DamageEffectParams.KnockbackForce);

	SetIsRadialDamage(EffectContextHandle, DamageEffectParams.bisRadialDamage);
	SetRadialDamageInnerRadius(EffectContextHandle, DamageEffectParams.RadialDamageInnerRadius);
	SetRadialDamageOuterRadius(EffectContextHandle, DamageEffectParams.RadialDamageOuterRadius);
	SetRadialDamageOrigin(EffectContextHandle, DamageEffectParams.RadialDamageOrigin);

	const FGameplayEffectSpecHandle SpecHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeOutgoingSpec(DamageEffectParams.DamageGamelayEffectClass, DamageEffectParams.AbilityLevel, EffectContextHandle);


	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageEffectParams.DamageType, DamageEffectParams.BaseDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTag.Debuff_Chance, DamageEffectParams.DebuffChance);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTag.Debuff_Damage, DamageEffectParams.DebuffDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTag.Debuff_Duration, DamageEffectParams.DebuffDuration);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTag.Debuff_Frequency, DamageEffectParams.DebuffFrequency);

	DamageEffectParams.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
	return EffectContextHandle;
}

TArray<FRotator> UTopDownRPGAbilitySystemLibrary::EvenlySpacedRotators(const FVector& Forward, const FVector& Axis, float Spread, int32 NumRotators)
{
	TArray<FRotator> Rotators;

	const FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread / 2.f, Axis);
	if (NumRotators > 1)
	{
		const float DeltaSpread = Spread / (NumRotators - 1);
		for (int32 i = 0; i < NumRotators; i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
			Rotators.Add(Direction.Rotation());
		}
	}
	else
	{
		Rotators.Add(Forward.Rotation());
	}

	return Rotators;
}

TArray<FVector> UTopDownRPGAbilitySystemLibrary::EvenlyRotatedVector(const FVector& Forward, const FVector& Axis, float Spread, int32 NumVectors)
{
	TArray<FVector> Vectors;

	const FVector LeftOfSpread = Forward.RotateAngleAxis(-Spread / 2.f, Axis);
	if (NumVectors > 1)
	{
		const float DeltaSpread = Spread / (NumVectors - 1);
		for (int32 i = 0; i < NumVectors; i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
			Vectors.Add(Direction);
		}
	}
	else
	{
		Vectors.Add(Forward);
	}

	return Vectors;
}


int32 UTopDownRPGAbilitySystemLibrary::GetXPRewardForClassAndLevel(const UObject* WorldContextObject, ECharacterClass CharacterClass, int32 CharacterLevel)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	if (CharacterClassInfo == nullptr)
	{
		return 0;
	}
	const FCharacterClassDefaultInfo Info = CharacterClassInfo->GetClassDefaultInfo(CharacterClass);
	const float XPReward = Info.XPReward.GetValueAtLevel(CharacterLevel);

	return static_cast<int32>(XPReward);
}

void UTopDownRPGAbilitySystemLibrary::SetIsRadialDamgeEffectParams(UPARAM(ref)FDamageEffectParams& DamageEffectParams, bool bIsRadial, float InnerRaidal, float OuterRadial, FVector Origin)
{
	DamageEffectParams.bisRadialDamage = bIsRadial;
	DamageEffectParams.RadialDamageInnerRadius = InnerRaidal;
	DamageEffectParams.RadialDamageOuterRadius = OuterRadial;
	DamageEffectParams.RadialDamageOrigin = Origin;
}

void UTopDownRPGAbilitySystemLibrary::SetKnockbackDirection(UPARAM(ref)FDamageEffectParams& DamageEffectParams, FVector KnockbackDirection, float Magnitude)
{
	KnockbackDirection.Normalize();
	if (Magnitude == 0.f)
	{
		DamageEffectParams.KnockbackForce = KnockbackDirection * DamageEffectParams.KnockbackForceMagnitude;
	}
	else
	{
		DamageEffectParams.KnockbackForce = KnockbackDirection * Magnitude;

	}
}

void UTopDownRPGAbilitySystemLibrary::SetDeathImpulseDirection(UPARAM(ref)FDamageEffectParams& DamageEffectParams, FVector ImpulseDirection, float Magnitude)
{
	ImpulseDirection.Normalize();
	if (Magnitude == 0.f)
	{
		DamageEffectParams.DeathImpulse = ImpulseDirection * DamageEffectParams.DeathImpulseMagnitude;

	}
	else
	{
		DamageEffectParams.DeathImpulse = ImpulseDirection * Magnitude;

	}
}

void UTopDownRPGAbilitySystemLibrary::SetTargetEffectParamsASC(UPARAM(ref)FDamageEffectParams& DamageEffectParams, UAbilitySystemComponent* InTargetASC)
{
	DamageEffectParams.TargetAbilitySystemComponent = InTargetASC;
}
