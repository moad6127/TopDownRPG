// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "AbilitySystem/Data/CharacterClassInfo.h"
#include "EnemySpawnPoint.generated.h"

/**
 * 
 */
class AEnemyCharacter;

UCLASS()
class AURA_API AEnemySpawnPoint : public ATargetPoint
{
	GENERATED_BODY()
public:

	UFUNCTION(BlueprintCallable)
	void SpawnEnemy();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy class")
	TSubclassOf<AEnemyCharacter>EnemyClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy class")
	int32 EnemyLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy class")
	ECharacterClass CharacterClass = ECharacterClass::Warrior;
};
