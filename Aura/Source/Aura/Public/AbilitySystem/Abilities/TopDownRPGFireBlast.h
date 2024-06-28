// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/TopDownRPGDamageGameplayAbility.h"
#include "TopDownRPGFireBlast.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UTopDownRPGFireBlast : public UTopDownRPGDamageGameplayAbility
{
	GENERATED_BODY()

public:

	virtual FString GetDescription(int32 Level) override;
	virtual FString GetNextLevelDescription(int32 Level) override;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Fire Blast")
	int32 NumFireBalls = 12;
	
};
