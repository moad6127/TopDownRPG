// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/TopDownRPGWidgetController.h"
#include "Player/TopDownRPGPlayerController.h"
#include "Player/TopDownRPGPlayerState.h"
#include "AbilitySystem/TopDownRPGAbilitySystemComponent.h"
#include "AbilitySystem/TopDownRPGAttributeSet.h"


void UTopDownRPGWidgetController::SetWidgetControllerParams(const FWidgetControllerParams& WCParams)
{
	PlayerController = WCParams.PlayerController;
	PlayerState = WCParams.PlayerState;
	AbilitySystemComponent = WCParams.AbilitySystemComponent;
	AttributeSet = WCParams.AttributeSet;
}

void UTopDownRPGWidgetController::BroadcastInitialValue()
{

}

void UTopDownRPGWidgetController::BindCallbacksToDependencies()
{
}

void UTopDownRPGWidgetController::BroadcastAbilityInfo()
{
	if (!GetTopDownRPGASC()->bStartupAbilitiesGiven)
	{
		return;
	}
	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this](const FGameplayAbilitySpec& AbilitySpec)
		{
			FTopDownRPGAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(TopDownRPGAbilitySystemComponent->GetAbilityTagFromSpec(AbilitySpec));
			Info.InputTag = TopDownRPGAbilitySystemComponent->GetInputTagFromSpec(AbilitySpec);
			Info.StatusTag = TopDownRPGAbilitySystemComponent->GetStatusFromSpec(AbilitySpec);
			AbilityInfoDelegate.Broadcast(Info);
		});
	GetTopDownRPGASC()->ForEachAbility(BroadcastDelegate);
}

ATopDownRPGPlayerController* UTopDownRPGWidgetController::GetTopDownRPGPC()
{
	if (TopDownRPGPlayerController == nullptr)
	{
		TopDownRPGPlayerController = Cast<ATopDownRPGPlayerController>(PlayerController);
	}
	return TopDownRPGPlayerController;
}

ATopDownRPGPlayerState* UTopDownRPGWidgetController::GetTopDownRPGPS()
{
	if (TopDownRPGPlayerState == nullptr)
	{
		TopDownRPGPlayerState = Cast<ATopDownRPGPlayerState>(PlayerState);
	}
	return TopDownRPGPlayerState;
}

UTopDownRPGAbilitySystemComponent* UTopDownRPGWidgetController::GetTopDownRPGASC()
{
	if (TopDownRPGAbilitySystemComponent == nullptr)
	{
		TopDownRPGAbilitySystemComponent = Cast<UTopDownRPGAbilitySystemComponent>(AbilitySystemComponent);
	}
	return TopDownRPGAbilitySystemComponent;
}

UTopDownRPGAttributeSet* UTopDownRPGWidgetController::GetTopDownRPGAS()
{
	if (TopDownRPGAttributeSet == nullptr)
	{
		TopDownRPGAttributeSet = Cast<UTopDownRPGAttributeSet>(AttributeSet);
	}
	return TopDownRPGAttributeSet;
}
