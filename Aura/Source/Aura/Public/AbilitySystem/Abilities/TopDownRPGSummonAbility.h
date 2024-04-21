// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/TopDownRPGGameplayAbility.h"
#include "TopDownRPGSummonAbility.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UTopDownRPGSummonAbility : public UTopDownRPGGameplayAbility
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	TArray<FVector> GetSpawnLocation();

	UFUNCTION(BlueprintPure, Category = "Summoning")
	TSubclassOf<APawn>GetRandomMinionClass();

	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	int32 NumMinions = 5;

	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	TArray<TSubclassOf<APawn>> MinionClasses;

	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	float MinSpawnDistance = 50.f;

	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	float MaxSpawnDistance = 250.f;

	UPROPERTY(EditDefaultsOnly, Category = "Summoning")
	float SpawnSpread = 90.f;

};
