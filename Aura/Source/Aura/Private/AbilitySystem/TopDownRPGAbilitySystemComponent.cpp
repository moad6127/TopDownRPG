// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/TopDownRPGAbilitySystemComponent.h"
#include "TopDownRPGGameplayTags.h"

void UTopDownRPGAbilitySystemComponent::AbilityActorInfoSet()
{
	OnGameplayEffectAppliedDelegateToSelf.AddUObject(this, &UTopDownRPGAbilitySystemComponent::EffectApplyed);

	const FTopDownRPGGameplayTags& GameplayTags = FTopDownRPGGameplayTags::Get();
	GEngine->AddOnScreenDebugMessage(
		-1,
		10.f,
		FColor::Orange,
		FString::Printf(TEXT("Tag: %s"), *GameplayTags.Attribute_Secondary_Armor.ToString()));
}

void UTopDownRPGAbilitySystemComponent::EffectApplyed(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& EffectSpec, FActiveGameplayEffectHandle ActiveEffectHandle)
{
	FGameplayTagContainer TagContainer;
	EffectSpec.GetAllAssetTags(TagContainer);
	EffectAssetTags.Broadcast(TagContainer);
}
