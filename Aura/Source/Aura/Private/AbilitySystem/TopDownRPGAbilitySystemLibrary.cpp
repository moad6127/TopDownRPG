// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/TopDownRPGAbilitySystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/TopDownRPGHUD.h"
#include "Player/TopDownRPGPlayerState.h"
#include "UI/WidgetController/TopDownRPGWidgetController.h"
#include "Game/TopDownRPGGameModeBase.h"
#include "AbilitySystemComponent.h"

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

void UTopDownRPGAbilitySystemLibrary::GiveStartupAbilities(const UObject* WorldContextObject, UAbilitySystemComponent* ASC)
{
	UCharacterClassInfo* CharacterClassInfo = GetCharacterClassInfo(WorldContextObject);
	for (TSubclassOf<UGameplayAbility> AbilityClass : CharacterClassInfo->CommonAbilities)
	{
		FGameplayAbilitySpec AbilitySpec = FGameplayAbilitySpec(AbilityClass, 1);
		ASC->GiveAbility(AbilitySpec);
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
