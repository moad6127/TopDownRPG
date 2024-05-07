// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TopDownRPGCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/TopDownRPGPlayerState.h"
#include "AbilitySystem/TopDownRPGAbilitySystemComponent.h"
#include "Player/TopDownRPGPlayerController.h"
#include "UI/HUD/TopDownRPGHUD.h"
#include "AbilitySystem/Data/LevelUpInfo.h"

ATopDownRPGCharacter::ATopDownRPGCharacter()
{
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 400.f, 0.f);
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->bSnapToPlaneAtStart = true;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	CharacterClass = ECharacterClass::Elemetalist;
}

void ATopDownRPGCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	//Init Ability Actor info for the server
	InitAbilityActorInfo();
	AddCharacterAbilities();
}

void ATopDownRPGCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	//Init Ability Actor info for the client
	InitAbilityActorInfo();
}

void ATopDownRPGCharacter::AddToXP_Implementation(int32 InXP)
{
	ATopDownRPGPlayerState* TopDownRPGPlayerState = GetPlayerState<ATopDownRPGPlayerState>();
	check(TopDownRPGPlayerState);
	TopDownRPGPlayerState->AddToXP(InXP);
}

void ATopDownRPGCharacter::LevelUp_Implementation()
{

}

int32 ATopDownRPGCharacter::GetXP_Implementation() const
{
	const ATopDownRPGPlayerState* TopDownRPGPlayerState = GetPlayerState<ATopDownRPGPlayerState>();
	check(TopDownRPGPlayerState);
	return TopDownRPGPlayerState->GetXP();
}

int32 ATopDownRPGCharacter::FindLevelForXP_Implementation(int32 InXP) const
{
	const ATopDownRPGPlayerState* TopDownRPGPlayerState = GetPlayerState<ATopDownRPGPlayerState>();
	check(TopDownRPGPlayerState);
	return TopDownRPGPlayerState->LevelUpInfo->FindLevelForXP(InXP);
}

int32 ATopDownRPGCharacter::GetAttributePointsReward_Implementation(int32 Level) const
{
	const ATopDownRPGPlayerState* TopDownRPGPlayerState = GetPlayerState<ATopDownRPGPlayerState>();
	check(TopDownRPGPlayerState);
	return TopDownRPGPlayerState->LevelUpInfo->LevelUpInformation[Level].AttributePointAward;
}

int32 ATopDownRPGCharacter::GetSpellPointsReward_Implementation(int32 Level) const
{
	const ATopDownRPGPlayerState* TopDownRPGPlayerState = GetPlayerState<ATopDownRPGPlayerState>();
	check(TopDownRPGPlayerState);
	return TopDownRPGPlayerState->LevelUpInfo->LevelUpInformation[Level].SpellPointAward;
}

void ATopDownRPGCharacter::AddToPlayerLevel_Implementation(int32 InPlayerLevel)
{
	ATopDownRPGPlayerState* TopDownRPGPlayerState = GetPlayerState<ATopDownRPGPlayerState>();
	check(TopDownRPGPlayerState);
	TopDownRPGPlayerState->AddToLevel(InPlayerLevel);
}

void ATopDownRPGCharacter::AddToAttributePoints_Implementation(int32 InAttribuetPoints)
{
	//TODO : AddToAttributePoint 를 PlayerState에 만들기
}

void ATopDownRPGCharacter::AddToSpellPoints_Implementation(int32 InSpellPoints)
{
	//TODO : AddToSpellPoint 를 PlayerState에 만들기
}

int32 ATopDownRPGCharacter::GetPlayerLevel_Implementation()
{
	const ATopDownRPGPlayerState* TopDownRPGPlayerState = GetPlayerState<ATopDownRPGPlayerState>();
	check(TopDownRPGPlayerState);

	return TopDownRPGPlayerState->GetPlayerLevel();
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
