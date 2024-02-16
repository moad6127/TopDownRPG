// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AbilitySystem/TopDownRPGAttributeSet.h"
#include "TopDownRPGGameplayTags.h"

void UAttributeMenuWidgetController::BroadcastInitialValue()
{
	UTopDownRPGAttributeSet* AS = CastChecked<UTopDownRPGAttributeSet>(AttributeSet);

	check(AttributeInfo);

	FTopDownRPGAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(FTopDownRPGGameplayTags::Get().Attributes_Primary_Strength);
	Info.AttributeValue = AS->GetStrength();
	AttributeInfoDelegate.Broadcast(Info);
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{

}
