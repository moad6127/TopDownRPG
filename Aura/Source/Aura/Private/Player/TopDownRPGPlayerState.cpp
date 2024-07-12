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
	DOREPLIFETIME(ATopDownRPGPlayerState, AttributePoint);
	DOREPLIFETIME(ATopDownRPGPlayerState, SpellPoint);
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
	OnLevelChangedDelegate.Broadcast(Level,false);
}


void ATopDownRPGPlayerState::SetAttributePoint(int32 InPoint)
{
	AttributePoint = InPoint;
	OnAttributePointsChangedDelegate.Broadcast(AttributePoint);
}

void ATopDownRPGPlayerState::SetSpellPoint(int32 InPoint)
{
	SpellPoint = InPoint;
	OnSpellPointsChangedDelegate.Broadcast(SpellPoint);
}

void ATopDownRPGPlayerState::AddToXP(int32 AddXP)
{
	XP += AddXP;
	OnXPChangedDelegate.Broadcast(XP);
}

void ATopDownRPGPlayerState::AddToLevel(int32 AddLevel)
{
	Level += AddLevel;
	OnLevelChangedDelegate.Broadcast(Level,true);
}

void ATopDownRPGPlayerState::AddToAttributePoints(int32 AddAttributePoint)
{
	AttributePoint += AddAttributePoint;
	OnAttributePointsChangedDelegate.Broadcast(AttributePoint);
}

void ATopDownRPGPlayerState::AddToSpellPoints(int32 AddSpellPoint)
{
	SpellPoint += AddSpellPoint;
	OnSpellPointsChangedDelegate.Broadcast(SpellPoint);
}

void ATopDownRPGPlayerState::OnRep_Level(int32 OldLevel)
{
	OnLevelChangedDelegate.Broadcast(Level, true);
}

void ATopDownRPGPlayerState::OnRep_XP(int32 OldXP)
{
	OnXPChangedDelegate.Broadcast(XP);
}

void ATopDownRPGPlayerState::OnRep_AttributePoint(int32 OldAttributePoint)
{
	OnAttributePointsChangedDelegate.Broadcast(AttributePoint);
}

void ATopDownRPGPlayerState::OnRep_SpellPoint(int32 OldSpellPoint)
{
	OnSpellPointsChangedDelegate.Broadcast(SpellPoint);
}


