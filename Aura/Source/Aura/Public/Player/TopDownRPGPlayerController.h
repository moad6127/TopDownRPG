// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameplayTagContainer.h"
#include "TopDownRPGPlayerController.generated.h"

/**
 * 
 */

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;
class IEnemyInterface;
class UTopDownRPGInputConfig;

UCLASS()
class AURA_API ATopDownRPGPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ATopDownRPGPlayerController();
	virtual void PlayerTick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:


	/*
	* Value;
	*/
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> CharacterContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	IEnemyInterface* LastActor;
	IEnemyInterface* ThisActor;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UTopDownRPGInputConfig> InputConfig;

	/*
	* Function
	*/

	void Move(const FInputActionValue& InputActionValue);
	void CursorTrace();

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);
};
