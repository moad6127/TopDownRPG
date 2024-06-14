// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TopDownRPGCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/TopDownRPGPlayerState.h"
#include "AbilitySystem/TopDownRPGAbilitySystemComponent.h"
#include "Player/TopDownRPGPlayerController.h"
#include "UI/HUD/TopDownRPGHUD.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "NiagaraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "TopDownRPGGameplayTags.h"


ATopDownRPGCharacter::ATopDownRPGCharacter()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>("CameraBoom");
	CameraBoom->SetupAttachment(GetRootComponent());
	CameraBoom->SetUsingAbsoluteRotation(true);
	CameraBoom->bDoCollisionTest = false;

	TopDownCameraComponent = CreateDefaultSubobject<UCameraComponent>("TopDownCameraComponen");
	TopDownCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	TopDownCameraComponent->bUsePawnControlRotation = false;

	LevelUpNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>("LevelUpNiagaraComponent");
	LevelUpNiagaraComponent->SetupAttachment(GetRootComponent());
	LevelUpNiagaraComponent->bAutoActivate = false;

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
	MulticastLevelUpParticles();
}

void ATopDownRPGCharacter::MulticastLevelUpParticles_Implementation() const
{
	if (IsValid(LevelUpNiagaraComponent))
	{
		const FVector CameraLocation = TopDownCameraComponent->GetComponentLocation();
		const FVector NiagaraSystemLocation = LevelUpNiagaraComponent->GetComponentLocation();
		const FRotator ToCameraRotaion = (CameraLocation - NiagaraSystemLocation).Rotation();

		LevelUpNiagaraComponent->SetWorldRotation(ToCameraRotaion);
		LevelUpNiagaraComponent->Activate(true);
	}
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

	if (UTopDownRPGAbilitySystemComponent* ASC = Cast<UTopDownRPGAbilitySystemComponent>(GetAbilitySystemComponent()))
	{
		ASC->UpdateAbilityStatues(TopDownRPGPlayerState->GetPlayerLevel());
	}
}

void ATopDownRPGCharacter::AddToAttributePoints_Implementation(int32 InAttribuetPoints)
{
	ATopDownRPGPlayerState* TopDownRPGPlayerState = GetPlayerState<ATopDownRPGPlayerState>();
	check(TopDownRPGPlayerState);
	TopDownRPGPlayerState->AddToAttributePoints(InAttribuetPoints);
}

void ATopDownRPGCharacter::AddToSpellPoints_Implementation(int32 InSpellPoints)
{
	ATopDownRPGPlayerState* TopDownRPGPlayerState = GetPlayerState<ATopDownRPGPlayerState>();
	check(TopDownRPGPlayerState);
	TopDownRPGPlayerState->AddToSpellPoints(InSpellPoints);
}

int32 ATopDownRPGCharacter::GetAttributePoints_Implementation() const
{
	ATopDownRPGPlayerState* TopDownRPGPlayerState = GetPlayerState<ATopDownRPGPlayerState>();
	check(TopDownRPGPlayerState);
	return TopDownRPGPlayerState->GetAttributePoints();
}

int32 ATopDownRPGCharacter::GetSpellPoints_Implementation() const
{
	ATopDownRPGPlayerState* TopDownRPGPlayerState = GetPlayerState<ATopDownRPGPlayerState>();
	check(TopDownRPGPlayerState);
	return TopDownRPGPlayerState->GetSpellPoints();
}

int32 ATopDownRPGCharacter::GetPlayerLevel_Implementation()
{
	const ATopDownRPGPlayerState* TopDownRPGPlayerState = GetPlayerState<ATopDownRPGPlayerState>();
	check(TopDownRPGPlayerState);

	return TopDownRPGPlayerState->GetPlayerLevel();
}

void ATopDownRPGCharacter::OnRep_Stunend()
{
	if (UTopDownRPGAbilitySystemComponent* ASC = Cast<UTopDownRPGAbilitySystemComponent>(AbilitySystemComponent))
	{
		const FTopDownRPGGameplayTags& GameplayTag = FTopDownRPGGameplayTags::Get();
		FGameplayTagContainer BlockedTags;
		BlockedTags.AddTag(GameplayTag.Player_Block_CursorTrace);
		BlockedTags.AddTag(GameplayTag.Player_Block_InputHeld);
		BlockedTags.AddTag(GameplayTag.Player_Block_InputPressed);
		BlockedTags.AddTag(GameplayTag.Player_Block_InputReleased);
		if (bIsStunned)
		{
			ASC->AddLooseGameplayTags(BlockedTags);
		}
		else
		{
			ASC->RemoveLooseGameplayTags(BlockedTags);
		}
	}
}

void ATopDownRPGCharacter::InitAbilityActorInfo()
{
	ATopDownRPGPlayerState* TopDownRPGPlayerState = GetPlayerState<ATopDownRPGPlayerState>();
	check(TopDownRPGPlayerState);
	TopDownRPGPlayerState->GetAbilitySystemComponent()->InitAbilityActorInfo(TopDownRPGPlayerState, this);
	Cast<UTopDownRPGAbilitySystemComponent>(TopDownRPGPlayerState->GetAbilitySystemComponent())->AbilityActorInfoSet();
	AbilitySystemComponent = TopDownRPGPlayerState->GetAbilitySystemComponent();
	AttributesSet = TopDownRPGPlayerState->GetAttributeSet();
	OnASCRegisterd.Broadcast(AbilitySystemComponent);
	AbilitySystemComponent->RegisterGameplayTagEvent(FTopDownRPGGameplayTags::Get().Debuff_Stun, EGameplayTagEventType::NewOrRemoved).AddUObject(this, &ATopDownRPGCharacter::StunTagChanged);

	if (ATopDownRPGPlayerController* TopDownPlayerController = Cast<ATopDownRPGPlayerController>(GetController()))
	{
		if (ATopDownRPGHUD* TopDownHUD = Cast<ATopDownRPGHUD>(TopDownPlayerController->GetHUD()))
		{
			TopDownHUD->InitOverlay(TopDownPlayerController, TopDownRPGPlayerState, AbilitySystemComponent, AttributesSet);
		}
	}

	InitializeDefaultAttributes();
}


