// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "TopDownRPGAIController.generated.h"


class UBlackboardComponent;
class UBehaviorTreeComponent;
/**
 * 
 */
UCLASS()
class AURA_API ATopDownRPGAIController : public AAIController
{
	GENERATED_BODY()
public:
	ATopDownRPGAIController();

protected:


	UPROPERTY()
	TObjectPtr<UBehaviorTreeComponent> BehaviorTreeComponent;
};
