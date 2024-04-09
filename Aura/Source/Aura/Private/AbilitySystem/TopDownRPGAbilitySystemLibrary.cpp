// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/TopDownRPGAbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/TopDownRPGHUD.h"
#include "Player/TopDownRPGPlayerState.h"
#include "UI/WidgetController/TopDownRPGWidgetController.h"
#include "Game/TopDownRPGGameModeBase.h"
#include "AbilitySystemComponent.h"
#include "TopDownRPGAbilityTypes.h"
#include "Interaction/CombatInterface.h"

UOverlayWidgetController* UTopDownRPGAbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ATopDownRPGHUD* TopDownRPGHUD = Cast<ATopDownRPGHUD>(PC->GetHUD()))
		{
			ATopDownRPGPlayerState* PS = PC->GetPlayerState<ATopDownRPGPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			return TopDownRPGHUD->GetOverlayWidgetController(WidgetControllerParams);
		}
	}
	return nullptr;
}

UAttributeMenuWidgetController* UTopDownRPGAbilitySystemLibrary::GetAttributeMenuController(const UObject* WorldContextObject)
{
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (ATopDownRPGHUD* TopDownRPGHUD = Cast<ATopDownRPGHUD>(PC->GetHUD()))
		{
			ATopDownRPGPlayerState* PS = PC->GetPlayerState<ATopDownRPGPlayerState>();
			UAbilitySystemComponent* ASC = PS->GetAbilitySystemComponent();
			UAttributeSet* AS = PS->GetAttributeSet();
			const FWidgetControllerParams WidgetControllerParams(PC, PS, ASC, AS);
			return TopDownRPGHUD->GetAttributeMenuController(WidgetControllerParams);
		}
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
		if (ICombatInterface* CombatInterface = Cast<ICombatInterface>(ASC->GetAvatarActor()))
		{
			FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, CombatInterface->GetPlayerLevel());;
			ASC->GiveAbility(AbilitySpec);
		}
	}
}

UCharacterClassInfo* UTopDownRPGAbilitySystemLibrary::GetCharacterClassInfo(const UObject* WorldContextObject)
{
	ATopDownRPGGameModeBase* TopDownRPGGameMode = Cast<ATopDownRPGGameModeBase>(UGameplayStatics::GetGameMode(WorldContextObject));
	if (TopDownRPGGameMode == nullptr)
	{
		return nullptr;
	}
	return TopDownRPGGameMode->CharacterClassInfo;
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
