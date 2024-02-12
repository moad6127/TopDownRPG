// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/AssetManager.h"
#include "TopDownRPGAssetManager.generated.h"

/**
 * 
 */
UCLASS()
class AURA_API UTopDownRPGAssetManager : public UAssetManager
{
	GENERATED_BODY()
public:
	static UTopDownRPGAssetManager& Get();

protected:

	virtual void StartInitialLoading();
};
