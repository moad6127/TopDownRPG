// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AbilitySystem/TopDownRPGAttributeSet.h"
#include "TopDownRPGGameplayTags.h"

void UAttributeMenuWidgetController::BroadcastInitialValue()
{
	UTopDownRPGAttributeSet* AS = CastChecked<UTopDownRPGAttributeSet>(AttributeSet);

	check(AttributeInfo);


	for (auto& Pair : AS->TagsToAttributes)
	{
		FTopDownRPGAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(Pair.Key);
		Info.AttributeValue = Pair.Value().GetNumericValue(AS);
		AttributeInfoDelegate.Broadcast(Info);
	}
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{

}
