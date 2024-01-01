// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/TopDownRPGCharacterBase.h"
#include "TopDownRPGCharacter.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API ATopDownRPGCharacter : public ATopDownRPGCharacterBase
{
	GENERATED_BODY()

public:
	ATopDownRPGCharacter();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

private:
	/*
	* Functions
	*/

	void InitAbilityActorInfo();

};
