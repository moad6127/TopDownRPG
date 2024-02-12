// Fill out your copyright notice in the Description page of Project Settings.


#include "TopDownRPGGameplayTags.h"
#include "GameplayTagsManager.h"

FTopDownRPGGameplayTags FTopDownRPGGameplayTags::GameplayTags;

void FTopDownRPGGameplayTags::InitializeNativeGameplayTags()
{
	UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Armor"), FString("Reduces damage taken, Improves Block Chance"));

}
