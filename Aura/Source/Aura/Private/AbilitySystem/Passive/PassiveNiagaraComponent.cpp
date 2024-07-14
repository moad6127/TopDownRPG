// Fill out your copyright notice in the Description page of Project Settings.
#include "AbilitySystem/Passive/PassiveNiagaraComponent.h"
#include "AbilitySystem/TopDownRPGAbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "Interaction/CombatInterface.h"
#include "TopDownRPGGameplayTags.h"


UPassiveNiagaraComponent::UPassiveNiagaraComponent()
{
	bAutoActivate = false;
}

void UPassiveNiagaraComponent::BeginPlay()
{
	Super::BeginPlay();

	if (UTopDownRPGAbilitySystemComponent* ASC = Cast<UTopDownRPGAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner())))
	{
		ASC->ActivatePassiveEffect.AddUObject(this, &UPassiveNiagaraComponent::OnPassiveActivate);
		ActivateIfEquipped(ASC);
	}
	else if(ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetOwner()))
	{
		CombatInterface->GetOnASCRegisterdDelegate().AddLambda([this](UAbilitySystemComponent* ASC)
			{
				if (UTopDownRPGAbilitySystemComponent* TopDownRPGASC = Cast<UTopDownRPGAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetOwner())))
				{
					TopDownRPGASC->ActivatePassiveEffect.AddUObject(this, &UPassiveNiagaraComponent::OnPassiveActivate);
					ActivateIfEquipped(TopDownRPGASC);
				}
			});
	}
}

void UPassiveNiagaraComponent::OnPassiveActivate(const FGameplayTag& AbilityTag, bool bActivate)
{
	if (AbilityTag.MatchesTagExact(PassivSpellTag))
	{
		if (bActivate && !IsActive())
		{
			Activate();
		}
		else
		{
			Deactivate();
		}
	}
}

void UPassiveNiagaraComponent::ActivateIfEquipped(UTopDownRPGAbilitySystemComponent* ASC)
{
	if (ASC->bStartupAbilitiesGiven)
	{
		if (ASC->GetStatusFormAbilityTag(PassivSpellTag) == FTopDownRPGGameplayTags::Get().Abilities_Status_Equipped)
		{
			Activate();
		}
	}
}
