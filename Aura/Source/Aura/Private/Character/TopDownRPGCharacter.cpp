// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TopDownRPGCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/TopDownRPGPlayerState.h"
#include "AbilitySystem/TopDownRPGAbilitySystemComponent.h"
#include "Player/TopDownRPGPlayerController.h"
#include "UI/HUD/TopDownRPGHUD.h"

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
	Cast<UTopDownRPGAbilitySystemComponent>(TopDownRPGPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	AbilitySystemComponent = TopDownRPGPlayerState->GetAbilitySystemComponent();
	AttributesSet = TopDownRPGPlayerState->GetAttributeSet();

	if (ATopDownRPGPlayerController* TopDownPlayerController = Cast<ATopDownRPGPlayerController>(GetController()))
	{
		if (ATopDownRPGHUD* TopDownHUD = Cast<ATopDownRPGHUD>(TopDownPlayerController->GetHUD()))
		{
			TopDownHUD->InitOverlay(TopDownPlayerController, TopDownRPGPlayerState, AbilitySystemComponent, AttributesSet);
		}
	}

	InitializeDefaultAttributes();
}
