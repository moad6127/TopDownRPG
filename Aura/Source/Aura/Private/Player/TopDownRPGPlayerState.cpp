// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TopDownRPGPlayerState.h"

#include "AbilitySystem/TopDownRPGAbilitySystemComponent.h"
#include "AbilitySystem/TopDownRPGAttributeSet.h"
#include "Net/UnrealNetwork.h"

ATopDownRPGPlayerState::ATopDownRPGPlayerState()
{
	AbilitySystemComponent = CreateDefaultSubobject<UTopDownRPGAbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributesSet = CreateDefaultSubobject<UTopDownRPGAttributeSet>("AttributeSet");

	NetUpdateFrequency = 100.f;
}

void ATopDownRPGPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATopDownRPGPlayerState, Level);
}

UAbilitySystemComponent* ATopDownRPGPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ATopDownRPGPlayerState::OnRep_Level(int32 OldLevel)
{

}


