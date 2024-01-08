// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/TopDownRPGAttributeSet.h"

void UOverlayWidgetController::BroadcastInitialValue()
{
	const UTopDownRPGAttributeSet* TopDownRPGAttributeSet = CastChecked<UTopDownRPGAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(TopDownRPGAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(TopDownRPGAttributeSet->GetMaxHealth());
}
