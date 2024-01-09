// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/TopDownRPGAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValue()
{
	const UTopDownRPGAttributeSet* TopDownRPGAttributeSet = CastChecked<UTopDownRPGAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(TopDownRPGAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(TopDownRPGAttributeSet->GetMaxHealth());

}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	const UTopDownRPGAttributeSet* TopDownRPGAttributeSet = CastChecked<UTopDownRPGAttributeSet>(AttributeSet);

	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		TopDownRPGAttributeSet->GetHealthAttribute()).AddUObject(this, &UOverlayWidgetController::HealthChanged);
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		TopDownRPGAttributeSet->GetMaxHealthAttribute()).AddUObject(this, &UOverlayWidgetController::MaxHealthChanged);

}

void UOverlayWidgetController::HealthChanged(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UOverlayWidgetController::MaxHealthChanged(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}
