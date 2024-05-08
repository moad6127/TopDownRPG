// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WidgetController/OverlayWidgetController.h"
#include "AbilitySystem/TopDownRPGAttributeSet.h"
#include "AbilitySystem/TopDownRPGAbilitySystemComponent.h"
#include "Player/TopDownRPGPlayerState.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
//#include "AbilitySystem/Data/AbilityInfo.h"

void UOverlayWidgetController::BroadcastInitialValue()
{
	const UTopDownRPGAttributeSet* TopDownRPGAttributeSet = CastChecked<UTopDownRPGAttributeSet>(AttributeSet);

	OnHealthChanged.Broadcast(TopDownRPGAttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(TopDownRPGAttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(TopDownRPGAttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(TopDownRPGAttributeSet->GetMaxMana());
}

void UOverlayWidgetController::BindCallbacksToDependencies()
{
	ATopDownRPGPlayerState* TopDownRPGPlayerState = CastChecked<ATopDownRPGPlayerState>(PlayerState);
	TopDownRPGPlayerState->OnXPChangedDelegate.AddUObject(this, &UOverlayWidgetController::OnXPChanged);
	TopDownRPGPlayerState->OnLevelChangedDelegate.AddLambda(
		[this](int32 NewLevel) 
		{
			OnPlayerLevelChangedDelegate.Broadcast(NewLevel);
		}
	);

	const UTopDownRPGAttributeSet* TopDownRPGAttributeSet = CastChecked<UTopDownRPGAttributeSet>(AttributeSet);
	//~ Use Lamda
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
		TopDownRPGAttributeSet->GetHealthAttribute()).AddLambda(
			[this](const FOnAttributeChangeData& Data)
			{
				OnHealthChanged.Broadcast(Data.NewValue);
			}
		);	
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(TopDownRPGAttributeSet->GetMaxHealthAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) {OnMaxHealthChanged.Broadcast(Data.NewValue); });
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(TopDownRPGAttributeSet->GetManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) {	OnManaChanged.Broadcast(Data.NewValue); });
	AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(TopDownRPGAttributeSet->GetMaxManaAttribute()).AddLambda([this](const FOnAttributeChangeData& Data) {	OnMaxManaChanged.Broadcast(Data.NewValue); });
	
	if (UTopDownRPGAbilitySystemComponent* TopDownRPGASC = Cast<UTopDownRPGAbilitySystemComponent>(AbilitySystemComponent))
	{
		if (TopDownRPGASC->bStartupAbilitiesGiven)
		{
			OnInitializeStartupAbilities(TopDownRPGASC);
		}
		else
		{
			TopDownRPGASC->AbilitiesGivenDelegate.AddUObject(this, &UOverlayWidgetController::OnInitializeStartupAbilities);

		}
		TopDownRPGASC->EffectAssetTags.AddLambda(
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

void UOverlayWidgetController::OnInitializeStartupAbilities(UTopDownRPGAbilitySystemComponent* TopDownRPGASC)
{
	//TODO Get information about all given abilities, look up their ability info, and broadcast it to widget
	if (!TopDownRPGASC->bStartupAbilitiesGiven)
	{
		return;
	}
	FForEachAbility BroadcastDelegate;
	BroadcastDelegate.BindLambda([this, TopDownRPGASC](const FGameplayAbilitySpec& AbilitySpec)
		{
			//TODO need a way to figure out the ability tag for a given ability spec
			FTopDownRPGAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(TopDownRPGASC->GetAbilityTagFromSpec(AbilitySpec));
			Info.InputTag = TopDownRPGASC->GetInputTagFromSpec(AbilitySpec);
			AbilityInfoDelegate.Broadcast(Info);
		});
	TopDownRPGASC->ForEachAbility(BroadcastDelegate);
}

void UOverlayWidgetController::OnXPChanged(int32 NewXP) const
{
	const ATopDownRPGPlayerState* TopDownRPGPlayerState = CastChecked<ATopDownRPGPlayerState>(PlayerState);
	const ULevelUpInfo* LevelUpInfo = TopDownRPGPlayerState->LevelUpInfo;
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
