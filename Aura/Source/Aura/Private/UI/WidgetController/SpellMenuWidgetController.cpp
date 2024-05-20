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
	GetTopDownRPGASC()->AbilityStatusChanged.AddLambda([this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag) 
		{
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
		}
	);
}

void USpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
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

	bool bEnableSpellPoints = false;
	bool bEnableEquip = false;
	ShouldEnableButton(AbilityStatus, SpellPoints, bEnableSpellPoints, bEnableEquip);
	SpellGlobeSelectedDelegate.Broadcast(bEnableSpellPoints, bEnableEquip);
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
