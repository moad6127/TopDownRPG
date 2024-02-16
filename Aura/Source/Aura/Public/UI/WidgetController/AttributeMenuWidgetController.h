// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/WidgetController/TopDownRPGWidgetController.h"
#include "AttributeMenuWidgetController.generated.h"

/**
 * 
 */

UCLASS(BlueprintType, Blueprintable)
class AURA_API UAttributeMenuWidgetController : public UTopDownRPGWidgetController
{
	GENERATED_BODY()
	
public:
	virtual void BroadcastInitialValue() override;
	virtual void BindCallbacksToDependencies() override;
};
