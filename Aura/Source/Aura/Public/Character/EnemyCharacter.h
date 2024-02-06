// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/TopDownRPGCharacterBase.h"
#include "Interaction/EnemyInterface.h"
#include "EnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API AEnemyCharacter : public ATopDownRPGCharacterBase , public IEnemyInterface
{
	GENERATED_BODY()
	
public:
	AEnemyCharacter();

	//~ Enemy Interface
	virtual void HighlightActor() override;
	virtual void UnHighlightActor() override;
	//~ Enemy Interface

	//~ CombatInterface
	virtual int32 GetPlayerLevel() override;
	//~ CombatInterface

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Character Class Defaults")
	int32 Level = 1;
};
