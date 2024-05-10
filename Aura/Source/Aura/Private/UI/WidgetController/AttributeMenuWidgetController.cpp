// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/AttributeMenuWidgetController.h"
#include "AbilitySystem/TopDownRPGAttributeSet.h"
#include "TopDownRPGGameplayTags.h"
#include "Player/TopDownRPGPlayerState.h"

void UAttributeMenuWidgetController::BroadcastInitialValue()
{
	UTopDownRPGAttributeSet* AS = CastChecked<UTopDownRPGAttributeSet>(AttributeSet);

	check(AttributeInfo);


	for (auto& Pair : AS->TagsToAttributes)
	{
		BroadcastAttributeInfo(Pair.Key, Pair.Value());
	}
}

void UAttributeMenuWidgetController::BindCallbacksToDependencies()
{
	UTopDownRPGAttributeSet* AS = CastChecked<UTopDownRPGAttributeSet>(AttributeSet);

	check(AttributeInfo);
	for (auto& Pair : AS->TagsToAttributes)
	{
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Pair.Value()).AddLambda(
			[this, Pair](const FOnAttributeChangeData& Data)
			{
				BroadcastAttributeInfo(Pair.Key, Pair.Value());
			}
		);
	}

	ATopDownRPGPlayerState* TopDownRPGPlayerState = CastChecked<ATopDownRPGPlayerState>(PlayerState);
	TopDownRPGPlayerState->OnAttributePointsChangedDelegate.AddLambda(
		[this](int32 Points)
		{
			AttributePointsChangedDelegate.Broadcast(Points);
		}
	);
}

void UAttributeMenuWidgetController::BroadcastAttributeInfo(const FGameplayTag& AttributeTag, const FGameplayAttribute& Attribute) const
{
	FTopDownRPGAttributeInfo Info = AttributeInfo->FindAttributeInfoForTag(AttributeTag);
	Info.AttributeValue = Attribute.GetNumericValue(AttributeSet);
	AttributeInfoDelegate.Broadcast(Info);
}
