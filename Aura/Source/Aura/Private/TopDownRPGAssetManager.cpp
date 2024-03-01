// Fill out your copyright notice in the Description page of Project Settings.


#include "TopDownRPGAssetManager.h"
#include "TopDownRPGGameplayTags.h"
#include "AbilitySystemGlobals.h"

UTopDownRPGAssetManager& UTopDownRPGAssetManager::Get()
{
	check(GEngine);

	UTopDownRPGAssetManager* TopDownAssetManager = Cast<UTopDownRPGAssetManager>(GEngine->AssetManager);
	return *TopDownAssetManager;
}

void UTopDownRPGAssetManager::StartInitialLoading()
{
	Super::StartInitialLoading();
	FTopDownRPGGameplayTags::InitializeNativeGameplayTags();

	//this is required to use target data
	UAbilitySystemGlobals::Get().InitGlobalData();
}
