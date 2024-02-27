// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/TopDownRPGGameplayAbility.h"
#include "TopDownRPGProjectileSpell.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UTopDownRPGProjectileSpell : public UTopDownRPGGameplayAbility
{
	GENERATED_BODY()
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

};
