// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/TopDownRPGCharacterBase.h"
#include "Interaction/PlayerInterface.h"
#include "TopDownRPGCharacter.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API ATopDownRPGCharacter : public ATopDownRPGCharacterBase, public IPlayerInterface
{
	GENERATED_BODY()

public:
	ATopDownRPGCharacter();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	//~ PlayerInterface
	virtual void AddToXP_Implementation(int32 InXP) override;
	virtual void LevelUp_Implementation() override;
	//~ PlayerInterface
	
	//~ CombatInterface
	virtual int32 GetPlayerLevel_Implementation() override;
	//~ CombatInterface


private:
	/*
	* Functions
	*/

	void InitAbilityActorInfo() override;

};
