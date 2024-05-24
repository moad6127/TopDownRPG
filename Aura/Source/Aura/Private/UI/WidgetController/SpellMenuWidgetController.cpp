// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/SpellMenuWidgetController.h"
#include "AbilitySystem/TopDownRPGAbilitySystemComponent.h"
#include "Player/TopDownRPGPlayerState.h"
#include "TopDownRPGGameplayTags.h"

void USpellMenuWidgetController::BroadcastInitialValue()
{
	BroadcastAbilityInfo();

	SpellPointPointsChangedDelegate.Broadcast(GetTopDownRPGPS()->GetSpellPoints());;
}

void USpellMenuWidgetController::BindCallbacksToDependencies()
{
	GetTopDownRPGASC()->AbilityStatusChanged.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, int32 NewLevel) 
		{
			if (SelectedAbility.Ability.MatchesTagExact(AbilityTag))
			{
				SelectedAbility.Status = StatusTag;
				bool bEnableSpellPoints = false;
				bool bEnableEquip = false;
				ShouldEnableButton(StatusTag, CurrentSpellPoints, bEnableSpellPoints, bEnableEquip);
				FString Description;
				FString NextLevelDescription;
				GetTopDownRPGASC()->GetDescriptionByAbilityTag(AbilityTag, Description, NextLevelDescription);
				SpellGlobeSelectedDelegate.Broadcast(bEnableSpellPoints, bEnableEquip, Description, NextLevelDescription);
			}

			if (AbilityInfo)
			{
				FTopDownRPGAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
				Info.StatusTag = StatusTag;
				AbilityInfoDelegate.Broadcast(Info);
			}
		}
	);
	GetTopDownRPGPS()->OnSpellPointsChangedDelegate.AddLambda(
		[this](int32 Points)
		{
			SpellPointPointsChangedDelegate.Broadcast(Points);
			CurrentSpellPoints = Points;

			bool bEnableSpellPoints = false;
			bool bEnableEquip = false;
			ShouldEnableButton(SelectedAbility.Status, CurrentSpellPoints, bEnableSpellPoints, bEnableEquip);
			FString Description;
			FString NextLevelDescription;
			GetTopDownRPGASC()->GetDescriptionByAbilityTag(SelectedAbility.Ability, Description, NextLevelDescription);
			SpellGlobeSelectedDelegate.Broadcast(bEnableSpellPoints, bEnableEquip, Description, NextLevelDescription);
		}
	);
}

void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
	if (bWaitingForEquipSelection)
	{
		const FGameplayTag SelectedAbiltyType = AbilityInfo->FindAbilityInfoForTag(AbilityTag).AbilityType;
		StopWaitForEquipDelegate.Broadcast(SelectedAbiltyType);
		bWaitingForEquipSelection = false;
	}


	const FTopDownRPGGameplayTags GameplayTag = FTopDownRPGGameplayTags::Get();
	const int32 SpellPoints = GetTopDownRPGPS()->GetSpellPoints();
	FGameplayTag AbilityStatus;

	const bool bTagValied = AbilityTag.IsValid();
	const bool bTagNone = AbilityTag.MatchesTag(GameplayTag.Abilities_None);
	FGameplayAbilitySpec* AbilitySpec = GetTopDownRPGASC()->GetSpecFromAbilityTag(AbilityTag);
	const bool bSpecValied = AbilitySpec != nullptr;
	if (!bTagValied || bTagNone || !bSpecValied)
	{
		AbilityStatus = GameplayTag.Abilities_Status_Locked;
	}
	else
	{
		AbilityStatus = GetTopDownRPGASC()->GetStatusFromSpec(*AbilitySpec);
	}
	SelectedAbility.Ability = AbilityTag;
	SelectedAbility.Status = AbilityStatus;

	bool bEnableSpellPoints = false;
	bool bEnableEquip = false;
	ShouldEnableButton(AbilityStatus, SpellPoints, bEnableSpellPoints, bEnableEquip);
	FString Description;
	FString NextLevelDescription;
	GetTopDownRPGASC()->GetDescriptionByAbilityTag(AbilityTag, Description, NextLevelDescription);
	SpellGlobeSelectedDelegate.Broadcast(bEnableSpellPoints, bEnableEquip, Description, NextLevelDescription);
}

void USpellMenuWidgetController::SpendPointButtonPressed()
{
	GetTopDownRPGASC()->ServerSpendSpellPoint(SelectedAbility.Ability);
}

void USpellMenuWidgetController::GlobeDeselect()
{
	if (bWaitingForEquipSelection)
	{
		const FGameplayTag SelectedAbiltyType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityType;
		StopWaitForEquipDelegate.Broadcast(SelectedAbiltyType);
		bWaitingForEquipSelection = false;
	}

	SelectedAbility.Ability = FTopDownRPGGameplayTags::Get().Abilities_None;
	SelectedAbility.Status = FTopDownRPGGameplayTags::Get().Abilities_Status_Locked;

	SpellGlobeSelectedDelegate.Broadcast(false, false, FString(), FString());
}

void USpellMenuWidgetController::EquipButtonPressed()
{
	const FGameplayTag& AbilityType = AbilityInfo->FindAbilityInfoForTag(SelectedAbility.Ability).AbilityType;
	
	WaitForEquipDelegate.Broadcast(AbilityType);
	bWaitingForEquipSelection = true;
}

void USpellMenuWidgetController::ShouldEnableButton(const FGameplayTag& AbilityStatus, int32 SpellPoints, bool& bShouldEnableSpellPointsButton, bool& bShouldEnableEquipButton)
{
	const FTopDownRPGGameplayTags GameplayTag = FTopDownRPGGameplayTags::Get();
	
	bShouldEnableSpellPointsButton = false;
	bShouldEnableEquipButton = false;

	if (AbilityStatus.MatchesTagExact(GameplayTag.Abilities_Status_Equipped))
	{
		bShouldEnableEquipButton = true;
		if (SpellPoints > 0)
		{
			bShouldEnableSpellPointsButton = true;
		}
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTag.Abilities_Status_Eligible))
	{
		if (SpellPoints > 0)
		{
			bShouldEnableSpellPointsButton = true;
		}
	}
	else if (AbilityStatus.MatchesTagExact(GameplayTag.Abilities_Status_UnLocked))
	{
		bShouldEnableEquipButton = true;
		if (SpellPoints > 0)
		{
			bShouldEnableSpellPointsButton = true;
		}
	}
}
