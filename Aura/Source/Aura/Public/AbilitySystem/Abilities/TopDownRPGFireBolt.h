// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/TopDownRPGProjectileSpell.h"
#include "TopDownRPGFireBolt.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UTopDownRPGFireBolt : public UTopDownRPGProjectileSpell
{
	GENERATED_BODY()
public:

	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;
	
};
