// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"

/**
 * TopDownRPGGameplayTags
 * 
 * Singleton containing native Gameplay Tags
 */
struct FTopDownRPGGameplayTags
{
public:
	static const FTopDownRPGGameplayTags& Get() { return GameplayTags; }
	static void InitializeNativeGameplayTags();
protected:

private:
	static FTopDownRPGGameplayTags GameplayTags;
};
