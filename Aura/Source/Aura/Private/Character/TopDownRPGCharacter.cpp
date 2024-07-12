// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/TopDownRPGCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/TopDownRPGPlayerState.h"
#include "AbilitySystem/TopDownRPGAbilitySystemComponent.h"
#include "Player/TopDownRPGPlayerController.h"
#include "UI/HUD/TopDownRPGHUD.h"
#include "AbilitySystem/Data/LevelUpInfo.h"
#include "AbilitySystem/TopDownRPGAttributeSet.h"
#include "AbilitySystem/TopDownRPGAbilitySystemLibrary.h"
#include "AbilitySystem/Data/AbilityInfo.h"
#include "NiagaraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "TopDownRPGGameplayTags.h"
#include "AbilitySystem/Debuff/DebuffNiagaraComponent.h"
#include "Game/TopDownRPGGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Game/LoadScreenSaveGame.h"



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
	LoadProgress();

}

void ATopDownRPGCharacter::LoadProgress()
{
	ATopDownRPGGameModeBase* GameMode = Cast<ATopDownRPGGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (GameMode)
	{
		ULoadScreenSaveGame* SaveData = GameMode->RetrieveInGameSaveData();
		if (SaveData == nullptr)
		{
			return;
		}

		if (SaveData->bFirstTimeLoadIn)
		{
			InitializeDefaultAttributes();
			AddCharacterAbilities();
		}
		else
		{
			//TODO : Load int Abilities from disk

			if (ATopDownRPGPlayerState* TopDownPlayerState = Cast<ATopDownRPGPlayerState>(GetPlayerState()))
			{
				TopDownPlayerState->SetLevel(SaveData->PlayerLevel);
				TopDownPlayerState->SetXP(SaveData->XP);
				TopDownPlayerState->SetAttributePoint(SaveData->AttributePoint);
				TopDownPlayerState->SetSpellPoint(SaveData->SpellPoint);
			}
			UTopDownRPGAbilitySystemLibrary::InitializeDefaultsAttributesFromSaveData(this, AbilitySystemComponent, SaveData);
		}
	}
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

void ATopDownRPGCharacter::ShowMagicCircle_Implementation(UMaterialInterface* DecalMaterial)
{
	if (ATopDownRPGPlayerController* TopDownPlayerController = Cast<ATopDownRPGPlayerController>(GetController()))
	{
		TopDownPlayerController->ShowMagicCircle(DecalMaterial);
		TopDownPlayerController->bShowMouseCursor = false;
	}
}

void ATopDownRPGCharacter::HideMagicCircle_Implementation()
{
	if (ATopDownRPGPlayerController* TopDownPlayerController = Cast<ATopDownRPGPlayerController>(GetController()))
	{
		TopDownPlayerController->HideMagicCircle();
		TopDownPlayerController->bShowMouseCursor = true;
	}
}

void ATopDownRPGCharacter::SaveProgress_Implementation(const FName& CheckpointTag)
{
	ATopDownRPGGameModeBase* GameMode = Cast<ATopDownRPGGameModeBase>(UGameplayStatics::GetGameMode(this));
	if (GameMode)
	{
		ULoadScreenSaveGame* SaveData = GameMode->RetrieveInGameSaveData();
		if (SaveData == nullptr)
		{
			return;
		}
		SaveData->PlayerStartTag = CheckpointTag;
		if (ATopDownRPGPlayerState* TopDownPlayerState = Cast<ATopDownRPGPlayerState>(GetPlayerState()))
		{
			SaveData->PlayerLevel = TopDownPlayerState->GetPlayerLevel();
			SaveData->XP = TopDownPlayerState->GetXP();
			SaveData->SpellPoint = TopDownPlayerState->GetSpellPoints();
			SaveData->AttributePoint = TopDownPlayerState->GetAttributePoints();
		}
		SaveData->Strength = UTopDownRPGAttributeSet::GetStrengthAttribute().GetNumericValue(GetAttributeSet());
		SaveData->Intelligence = UTopDownRPGAttributeSet::GetIntelligenceAttribute().GetNumericValue(GetAttributeSet());
		SaveData->Resilience = UTopDownRPGAttributeSet::GetResilienceAttribute().GetNumericValue(GetAttributeSet());
		SaveData->Vigor = UTopDownRPGAttributeSet::GetVigorAttribute().GetNumericValue(GetAttributeSet());
		
		SaveData->bFirstTimeLoadIn = false;

		if (!HasAuthority())
		{
			return;
		}
		UTopDownRPGAbilitySystemComponent* TopDownASC = Cast<UTopDownRPGAbilitySystemComponent>(AbilitySystemComponent);
		FForEachAbility SaveAbilityDelegate;
		SaveAbilityDelegate.BindLambda([this, TopDownASC, &SaveData](const FGameplayAbilitySpec& AbilitySpec)
			{
				const FGameplayTag AbilityTag = TopDownASC->GetAbilityTagFromSpec(AbilitySpec);
				UAbilityInfo* AbilityInfo = UTopDownRPGAbilitySystemLibrary::GetAbilityInfo(this);
				FTopDownRPGAbilityInfo Info = AbilityInfo->FindAbilityInfoForTag(AbilityTag);

				FSavedAbility SavedAbility;
				SavedAbility.GameplayAbility = Info.Ability;
				SavedAbility.AbilityLevel = AbilitySpec.Level;
				SavedAbility.AbilitySlot = TopDownASC->GetSlotFromAbilityTag(AbilityTag);
				SavedAbility.AbilityStatus = TopDownASC->GetStatusFormAbilityTag(AbilityTag);
				SavedAbility.AbilityTag = AbilityTag;
				SavedAbility.AbilityType = Info.AbilityType;

				SaveData->SavedAbiliteis.Add(SavedAbility);
			});
		TopDownASC->ForEachAbility(SaveAbilityDelegate);

		GameMode->SaveInGameProgressData(SaveData);
	}
}

int32 ATopDownRPGCharacter::GetPlayerLevel_Implementation()
{
	const ATopDownRPGPlayerState* TopDownRPGPlayerState = GetPlayerState<ATopDownRPGPlayerState>();
	check(TopDownRPGPlayerState);

	return TopDownRPGPlayerState->GetPlayerLevel();
}

void ATopDownRPGCharacter::OnRep_Stunned()
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
			StunDebuffComponent->Activate();
		}
		else
		{
			ASC->RemoveLooseGameplayTags(BlockedTags);
			StunDebuffComponent->Deactivate();
		}
	}
}

void ATopDownRPGCharacter::OnRep_Burned()
{
	if (bIsBurned)
	{
		BurnDebuffComponent->Activate();
	}
	else
	{
		BurnDebuffComponent->Deactivate();
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
}


