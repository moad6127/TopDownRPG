// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TopDownRPGPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Interaction/EnemyInterface.h"
#include "Input/TopDownRPGInputComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/TopDownRPGAbilitySystemComponent.h"


ATopDownRPGPlayerController::ATopDownRPGPlayerController()
{
	bReplicates = true;
}

void ATopDownRPGPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();

}

void ATopDownRPGPlayerController::CursorTrace()
{
	FHitResult CursorHit;
	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit)
	{
		return;
	}	

	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());

	/*
	* 1. LastActor �� ThisActor��� null
	*	-> Do nothing
	* 
	* 2. LastActor�� null && ThisActor�� ����
	*	-> ThisActor �� Highlight
	* 
	* 3. LastActor���� && ThisActor null
	*	-> LastActor�� UnHighlight;
	* 
	* 4. �Ѵ� ���������� LastActor != ThisActor�϶�
	*	->LastActor UnHighlight, ThisActor Highlight;
	* 
	* 5. �Ѵ� �����ϰ� LastActor == ThisActor�϶�
	*	-> Do Nothing
	*/
	if (LastActor == nullptr)
	{
		if (ThisActor != nullptr)
		{
			// 2��
			ThisActor->HighlightActor();
		}
	}
	else //LastActor ����
	{
		if (ThisActor == nullptr)
		{
			//3��
			LastActor->UnHighlightActor();
		}
		else //�Ѵ� ����
		{
			if (LastActor != ThisActor)
			{
				//4��
				LastActor->UnHighlightActor();
				ThisActor->HighlightActor();
			}
		}
	}
}

void ATopDownRPGPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	//GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Red, *InputTag.ToString());
}

void ATopDownRPGPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (GetASC() == nullptr)
	{
		return;
	}
	GetASC()->AbilityInputTagReleased(InputTag);
}

void ATopDownRPGPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (GetASC() == nullptr)
	{
		return;
	}
	GetASC()->AbilityInputTagHeld(InputTag);
}

void ATopDownRPGPlayerController::BeginPlay()
{
	Super::BeginPlay();
	check(CharacterContext);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (Subsystem)
	{
		Subsystem->AddMappingContext(CharacterContext, 0);
	}
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;

	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}

void ATopDownRPGPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	UTopDownRPGInputComponent* TopDownRPGInputComponent = CastChecked<UTopDownRPGInputComponent>(InputComponent);

	TopDownRPGInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATopDownRPGPlayerController::Move);

	TopDownRPGInputComponent->BindAbilityActions(
		InputConfig,
		this,
		&ATopDownRPGPlayerController::AbilityInputTagPressed,
		&ATopDownRPGPlayerController::AbilityInputTagReleased,
		&ATopDownRPGPlayerController::AbilityInputTagHeld);
}

UTopDownRPGAbilitySystemComponent* ATopDownRPGPlayerController::GetASC()
{
	if (TopDownRPGAbilitySystemComponent == nullptr)
	{
		TopDownRPGAbilitySystemComponent = Cast<UTopDownRPGAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn<APawn>()));
	}
	return TopDownRPGAbilitySystemComponent;
}

void ATopDownRPGPlayerController::Move(const FInputActionValue& InputActionValue)
{
	const FVector2D InputAxisVector = InputActionValue.Get<FVector2D>();
	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	if (APawn* ControlledPawn = GetPawn<APawn>())
	{
		ControlledPawn->AddMovementInput(ForwardDirection, InputAxisVector.Y);
		ControlledPawn->AddMovementInput(RightDirection, InputAxisVector.X);
	}
}

