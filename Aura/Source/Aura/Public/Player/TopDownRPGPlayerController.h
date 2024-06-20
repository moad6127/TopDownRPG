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
class UTopDownRPGAbilitySystemComponent;
class USplineComponent;
class UDamageTextComponent;
class UNiagaraSystem;
class AMagicCircle;

UCLASS()
class AURA_API ATopDownRPGPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	ATopDownRPGPlayerController();
	virtual void PlayerTick(float DeltaTime) override;

	//~ FloatingDamage
	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(float DamageAmount, ACharacter* TargetCharacter, bool bBlockedHit, bool bCriticalHit);
	//~ FloatingDamage

	UFUNCTION(BlueprintCallable)
	void ShowMagicCircle();

	UFUNCTION(BlueprintCallable)
	void HideMagicCircle();
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> CharacterContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ShiftAction;
	bool bShiftKeyDown = false;

	IEnemyInterface* LastActor;
	IEnemyInterface* ThisActor;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UTopDownRPGInputConfig> InputConfig;

	UPROPERTY()
	TObjectPtr<UTopDownRPGAbilitySystemComponent> TopDownRPGAbilitySystemComponent;

	FHitResult CursorHit;


	//~ FloatingDamage
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UDamageTextComponent> DamageTextComponentClass;

	//~ FloatingDamage

	//~ Click To Move
	FVector CachedDestination = FVector::ZeroVector;
	float FollowTime = 0.f;
	float ShortPressThreshold = 0.5f;
	bool bAutoRunning = false;
	bool bTargeting = false;

	UPROPERTY(EditDefaultsOnly)
	float AutoRunAcceptanceRadius = 50.f;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USplineComponent> Spline;
	//~ Click To Move

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UNiagaraSystem> ClickNiagaraSystem;


	UTopDownRPGAbilitySystemComponent* GetASC();
	void Move(const FInputActionValue& InputActionValue);
	void ShiftPressed() { bShiftKeyDown = true; }
	void ShiftReleased() { bShiftKeyDown = false; }

	void CursorTrace();

	void AbilityInputTagPressed(FGameplayTag InputTag);
	void AbilityInputTagReleased(FGameplayTag InputTag);
	void AbilityInputTagHeld(FGameplayTag InputTag);

	void AutoRun();


	//~ MagicCircle
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AMagicCircle> MagicCircleClass;

	UPROPERTY()
	TObjectPtr<AMagicCircle> MagicCircle;

	void UpdateMagicCircleLocation();
};
