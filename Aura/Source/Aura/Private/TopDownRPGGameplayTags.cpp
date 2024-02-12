// Fill out your copyright notice in the Description page of Project Settings.


#include "TopDownRPGGameplayTags.h"
#include "GameplayTagsManager.h"

FTopDownRPGGameplayTags FTopDownRPGGameplayTags::GameplayTags;

void FTopDownRPGGameplayTags::InitializeNativeGameplayTags()
{
	GameplayTags.Attribute_Secondary_Armor = UGameplayTagsManager::Get().AddNativeGameplayTag(FName("Attributes.Secondary.Armor"), FString("Reduces damage taken, Improves Block Chance"));

}
