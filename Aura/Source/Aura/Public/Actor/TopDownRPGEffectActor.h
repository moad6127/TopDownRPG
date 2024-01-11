// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TopDownRPGEffectActor.generated.h"

class UGameplayEffect;

UCLASS()
class AURA_API ATopDownRPGEffectActor : public AActor
{
	GENERATED_BODY()
	
public:	
	ATopDownRPGEffectActor();

protected:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void ApplayEffectToTarget(AActor* TargetActor, TSubclassOf<UGameplayEffect> GameplayEffectClass);

	UPROPERTY(EditAnywhere,BlueprintReadOnly, Category ="Applied Effects")
	TSubclassOf<UGameplayEffect> InstnaceGameplayEffectClass;
};
