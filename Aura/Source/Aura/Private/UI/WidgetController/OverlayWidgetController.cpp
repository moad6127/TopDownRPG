// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/TopDownRPGAttributeSet.h"
#include "AbilitySystem/TopDownRPGAbilitySystemComponent.h"

void UOverlayWidgetController::BroadcastInitialValue()
{
	const UTopDownRPGAttributeSet* TopDownRPGAttributeSet = CastChecked<UTopDownRPGAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(TopDownRPGAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(TopDownRPGAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(TopDownRPGAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(TopDownRPGAttributeSet->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UTopDownRPGAttributeSet* TopDownRPGAttributeSet = CastChecked<UTopDownRPGAttributeSet>(AttributeSet);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		TopDownRPGAttributeSet->GetHealthAttribute()).AddUObject(this, &UOverlayWidgetController::HealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		TopDownRPGAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &UOverlayWidgetController::MaxHealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		TopDownRPGAttributeSet->GetManaAttribute()).AddUObject(this, &UOverlayWidgetController::ManaChaned);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		TopDownRPGAttributeSet->GetMaxManaAttribute()).AddUObject(this, &UOverlayWidgetController::MaxManaChaned);
	
	Cast<UTopDownRPGAbilitySystemComponent>(AbilitySystemComponent)->EffectAssetTags.AddLambda(
		[this](const FGameplayTagContainer& AssetTags)
		{
			for (const FGameplayTag& tag : AssetTags)
			{
				FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
				if (tag.MatchesTag(MessageTag))
				{
					const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, tag);
					MessageWidgetRowDelegate.Broadcast(*Row);
				}
			}
		}
	);
}

void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::ManaChaned(const FOnAttributeChangeData& Data) const
{
	OnManaChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxManaChaned(const FOnAttributeChangeData& Data) const
{
	OnMaxManaChanged.Broadcast(Data.NewValue);
}
