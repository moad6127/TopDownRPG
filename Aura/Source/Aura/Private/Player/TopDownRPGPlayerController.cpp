// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TopDownRPGPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Interaction/EnemyInterface.h"


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
	* 1. LastActor 와 ThisActor모두 null
	*	-> Do nothing
	* 
	* 2. LastActor은 null && ThisActor은 존재
	*	-> ThisActor 에 Highlight
	* 
	* 3. LastActor존재 && ThisActor null
	*	-> LastActor에 UnHighlight;
	* 
	* 4. 둘다 존재하지만 LastActor != ThisActor일때
	*	->LastActor UnHighlight, ThisActor Highlight;
	* 
	* 5. 둘다 존재하고 LastActor == ThisActor일때
	*	-> Do Nothing
	*/
	if (LastActor == nullptr)
	{
		if (ThisActor != nullptr)
		{
			// 2번
			ThisActor->HighlightActor();
		}
	}
	else //LastActor 존재
	{
		if (ThisActor == nullptr)
		{
			//3번
			LastActor->UnHighlightActor();
		}
		else //둘다 존재
		{
			if (LastActor != ThisActor)
			{
				//4번
				LastActor->UnHighlightActor();
				ThisActor->HighlightActor();
			}
		}
	}
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

	UEnhancedInputComponent* EnhancedInputCompoent = CastChecked<UEnhancedInputComponent>(InputComponent);

	EnhancedInputCompoent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATopDownRPGPlayerController::Move);
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

