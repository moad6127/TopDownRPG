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
	DOREPLIFETIME(ATopDownRPGPlayerState, XP);
}

UAbilitySystemComponent* ATopDownRPGPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ATopDownRPGPlayerState::SetXP(int32 InXP)
{
	XP = InXP;
	OnXPChangedDelegate.Broadcast(XP);
}

void ATopDownRPGPlayerState::SetLevel(int32 InLevel)
{
	Level = InLevel;
	OnLevelChangedDelegate.Broadcast(Level);
}

void ATopDownRPGPlayerState::AddToXP(int32 AddXP)
{
	XP += AddXP;
	OnXPChangedDelegate.Broadcast(XP);
}

void ATopDownRPGPlayerState::AddToLevel(int32 AddLevel)
{
	Level += AddLevel;
	OnLevelChangedDelegate.Broadcast(Level);
}

void ATopDownRPGPlayerState::OnRep_Level(int32 OldLevel)
{
	OnLevelChangedDelegate.Broadcast(Level);
}

void ATopDownRPGPlayerState::OnRep_XP(int32 OldXP)
{
	OnXPChangedDelegate.Broadcast(XP);
}


