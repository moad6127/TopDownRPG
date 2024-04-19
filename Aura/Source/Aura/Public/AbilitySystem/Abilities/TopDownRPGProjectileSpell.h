// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TopDownRPGDamageGameplayAbility.h"
#include "TopDownRPGProjectileSpell.generated.h"


class ATopDownRPGProjectile;
class UGameplayEffect;
struct FGameplayTag;
/**
 * 
 */
UCLASS()
class AURA_API UTopDownRPGProjectileSpell : public UTopDownRPGDamageGameplayAbility
{
	GENERATED_BODY()
	
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	UFUNCTION(BlueprintCallable, Category = "Projectile")
	void SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocektTag);

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ATopDownRPGProjectile> ProjectileClass;

};
