// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TopDownRPGPlayerController.generated.h"

/**
 * 
 */

class UInputMappingContext;

UCLASS()
class AURA_API ATopDownRPGPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ATopDownRPGPlayerController();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> CharacterContext;

};
