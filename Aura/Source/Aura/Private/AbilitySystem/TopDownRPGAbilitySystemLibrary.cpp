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
	const FGameplayEffectSpecHandle SpecHandle = DamageEffectParams.SourceAbilitySystemComponent->MakeOutgoingSpec(DamageEffectParams.DamageGamelayEffectClass, DamageEffectParams.AbilityLevel, EffectContextHandle);

	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, DamageEffectParams.DamageType, DamageEffectParams.BaseDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTag.Debuff_Chance, DamageEffectParams.DebuffChance);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTag.Debuff_Damage, DamageEffectParams.DebuffDamage);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTag.Debuff_Duration, DamageEffectParams.DebuffDuration);
	UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, GameplayTag.Debuff_Frequency, DamageEffectParams.DebuffFrequency);


	DamageEffectParams.TargetAbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data);
	return EffectContextHandle;
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
