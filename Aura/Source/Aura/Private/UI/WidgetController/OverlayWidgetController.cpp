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

	//~ Use Lamda
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		TopDownRPGAttributeSet->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(TopDownRPGAttributeSet->GetMaxHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) {OnMaxHealthChanged.Broadcast(Data.NewValue); });
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(TopDownRPGAttributeSet->GetManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) {	OnManaChanged.Broadcast(Data.NewValue); });
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(TopDownRPGAttributeSet->GetMaxManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) {	OnMaxManaChanged.Broadcast(Data.NewValue); });
	
	if (UTopDownRPGAbilitySystemComponent* TopDownRPGASC = Cast<UTopDownRPGAbilitySystemComponent>(AbilitySystemComponent))
	{
		if (TopDownRPGASC->bStartupAbilitiesGiven)
		{
			OnInitializeStartupAbilities(TopDownRPGASC);
		}
		else
		{
			TopDownRPGASC->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::OnInitializeStartupAbilities);

		}
		TopDownRPGASC->EffectAssetTags.AddLambda(
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

}

void UOverlayWidgetController::OnInitializeStartupAbilities(UTopDownRPGAbilitySystemComponent* TopDownRPGASC)
{
	//TODO Get information about all given abilities, look up their ability info, and broadcast it to widget
	if (!TopDownRPGASC->bStartupAbilitiesGiven)
	{
		return;
	}
}
