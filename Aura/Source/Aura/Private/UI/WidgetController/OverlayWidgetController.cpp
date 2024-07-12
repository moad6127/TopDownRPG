// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/TopDownRPGAttributeSet.h"
#include "AbilitySystem/TopDownRPGAbilitySystemComponent.h"
#include "Player/TopDownRPGPlayerState.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "TopDownRPGGameplayTags.h"


void UOverlayWidgetController::BroadcastInitialValue()
{


	OnHealthChanged.Broadcast(GetTopDownRPGAS()->GetHealth());
	OnMaxHealthChanged.Broadcast(GetTopDownRPGAS()->GetMaxHealth());
	OnManaChanged.Broadcast(GetTopDownRPGAS()->GetMana());
	OnMaxManaChanged.Broadcast(GetTopDownRPGAS()->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	GetTopDownRPGPS()->OnXPChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnXPChanged);
	GetTopDownRPGPS()->OnLevelChangedDelegate.AddLambda(
		[this](int32 NewLevel,bool bLevelUp) 
		{
			OnPlayerLevelChangedDelegate.Broadcast(NewLevel,bLevelUp);
		}
	);

	//~ Use Lamda
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		GetTopDownRPGAS()->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetTopDownRPGAS()->GetMaxHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) {OnMaxHealthChanged.Broadcast(Data.NewValue); });
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetTopDownRPGAS()->GetManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) {	OnManaChanged.Broadcast(Data.NewValue); });
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(GetTopDownRPGAS()->GetMaxManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) {	OnMaxManaChanged.Broadcast(Data.NewValue); });
	
	if (GetTopDownRPGASC())
	{
		GetTopDownRPGASC()->AbilityEquipped.AddUObject(this, &UOverlayWidgetController::OnAbilityEquipped);
		if (GetTopDownRPGASC()->bStartupAbilitiesGiven)
		{
			BroadcastAbilityInfo();
		}
		else
		{
			GetTopDownRPGASC()->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::BroadcastAbilityInfo);

		}
		GetTopDownRPGASC()->EffectAssetTags.AddLambda(
			[this](const FGameplayTagContainer& AssetTags)
			{
				for (const FGameplayTag& tag : AssetTags)
				{
					FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag(FName("Message"));
					if (tag.MatchesTag(MessageTag))
					{
						const FUIWidgetRow* Row = GetDataTableRowByTag<FUIWidgetRow>(MessageWidgetDataTable, tag);
						MessageWidgetRowDelegate.Broadcast(*Row);
					}
				}
			}
		);
	}

}


void UOverlayWidgetController::OnXPChanged(int32 NewXP)
{
	const ULevelUpInfo* LevelUpInfo = GetTopDownRPGPS()->LevelUpInfo;
	checkf(LevelUpInfo, TEXT("Unable to find LevelUpInfo!!"));
	
	const int32 Level = LevelUpInfo->FindLevelForXP(NewXP);
	const int32 MaxLevel = LevelUpInfo->LevelUpInformation.Num();


	if (Level <= MaxLevel && Level > 0)
	{
		const int32 LevelUpRequirement = LevelUpInfo->LevelUpInformation[Level].LevelUpRequirement;
		const int32 PrevLevelUpRequirement = LevelUpInfo->LevelUpInformation[Level - 1].LevelUpRequirement;

		const int32 DeltaLevelReq = LevelUpRequirement - PrevLevelUpRequirement;
		const int32 XPForThisLevel = NewXP - PrevLevelUpRequirement;

		const float XPBarPercent = static_cast<float>(XPForThisLevel) / DeltaLevelReq;

		OnXPPercentChangedDelegate.Broadcast(XPBarPercent);
	}
}

void UOverlayWidgetController::OnAbilityEquipped(const FGameplayTag& AbilityTag, const FGameplayTag& Status, const FGameplayTag& Slot, const FGameplayTag& PrevSlot) const
{

	const FTopDownRPGGameplayTags& GameplayTag = FTopDownRPGGameplayTags::Get();

	FTopDownRPGAbilityInfo LastSlotInfo;
	LastSlotInfo.StatusTag = GameplayTag.Abilities_Status_UnLocked;
	LastSlotInfo.InputTag = PrevSlot;
	LastSlotInfo.AbilityTag = GameplayTag.Abilities_None;
	//PrevSlot이 존재할때 비어있는 Info를 Broadcast한다.
	//PrevSlot을 Clear한다.
	AbilityInfoDelegate.Broadcast(LastSlotInfo);

	FTopDownRPGAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);
	Info.StatusTag = Status;
	Info.InputTag = Slot;
	AbilityInfoDelegate.Broadcast(Info);
}
