// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TopDownRPGCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/TopDownRPGPlayerState.h"
#include "AbilitySystem/TopDownRPGAbilitySystemComponent.h"

ATopDownRPGCharacter::ATopDownRPGCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
}

void ATopDownRPGCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//Init Ability Actor info for the server
	InitAbilityActorInfo();
}

void ATopDownRPGCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	//Init Ability Actor info for the client
	InitAbilityActorInfo();
}

void ATopDownRPGCharacter::InitAbilityActorInfo()
{
	ATopDownRPGPlayerState* TopDownRPGPlayerState = GetPlayerState<ATopDownRPGPlayerState>();
	check(TopDownRPGPlayerState);
	TopDownRPGPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(TopDownRPGPlayerState, this);
	AbilitySystemComponent = TopDownRPGPlayerState->GetAbilitySystemComponent();
	AttributesSet = TopDownRPGPlayerState->GetAttributeSet();
}
