// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TopDownRPGPlayerState.h"

#include "AbilitySystem/TopDownRPGAbilitySystemComponent.h"
#include "AbilitySystem/TopDownRPGAttributeSet.h"

ATopDownRPGPlayerState::ATopDownRPGPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UTopDownRPGAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);

	AttributesSet = CreateDefaultSubobject<UTopDownRPGAttributeSet>("AttributeSet");

	NetUpdateFrequency = 100.f;
}

UAbilitySystemComponent* ATopDownRPGPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}


