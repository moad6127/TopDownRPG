// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TopDownRPGHUD.generated.h"

/**
 * 
 */

class UTopDownRPGUserWidget;

UCLASS()
class AURA_API ATopDownRPGHUD : public AHUD
{
	GENERATED_BODY()
	
public:

	UPROPERTY()
	TObjectPtr<UTopDownRPGUserWidget> OverlayWidget;

protected:

	virtual void BeginPlay() override;
private:

	UPROPERTY(EditAnywhere)
	TSubclassOf<UTopDownRPGUserWidget> OverlayWidgetClass;
};
