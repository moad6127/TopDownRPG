// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TopDownRPGCharacterBase.generated.h"

UCLASS(Abstract)
class AURA_API ATopDownRPGCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ATopDownRPGCharacterBase();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Combat")
	TObjectPtr<USkeletalMeshComponent> Weapon;

};
