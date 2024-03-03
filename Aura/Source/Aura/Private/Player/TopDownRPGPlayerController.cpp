// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/TopDownRPGPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Interaction/EnemyInterface.h"
#include "Input/TopDownRPGInputComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/TopDownRPGAbilitySystemComponent.h"
#include "Components/SplineComponent.h"
#include "TopDownRPGGameplayTags.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"


ATopDownRPGPlayerController::ATopDownRPGPlayerController()
{
	bReplicates = true;

	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
}

void ATopDownRPGPlayerController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);
	CursorTrace();
	AutoRun();

}

void ATopDownRPGPlayerController::AutoRun()
{
	if (!bAutoRunning)
	{
		return;
	}
	if (APawn* ControlledPawn = GetPawn())
	{
		const FVector LocationOnSpline = Spline->FindLocationClosestToWorldLocation(ControlledPawn->GetActorLocation(), ESplineCoordinateSpace::World);
		const FVector Direction = Spline->FindDirectionClosestToWorldLocation(LocationOnSpline, ESplineCoordinateSpace::World);
		ControlledPawn->AddMovementInput(Direction);

		const float DistanceToDestination = (LocationOnSpline - CachedDestination).Length();
		if (DistanceToDestination <= AutoRunAcceptanceRadius)
		{
			bAutoRunning = false;
		}
	}
}

void ATopDownRPGPlayerController::CursorTrace()
{

	GetHitResultUnderCursor(ECC_Visibility, false, CursorHit);
	if (!CursorHit.bBlockingHit)
	{
		return;
	}	

	LastActor = ThisActor;
	ThisActor = Cast<IEnemyInterface>(CursorHit.GetActor());

	if (LastActor != ThisActor)
	{
		if (LastActor)
		{
			LastActor->UnHighlightActor();
		}
		if (ThisActor)
		{
			ThisActor->HighlightActor();
		}
	}
}

void ATopDownRPGPlayerController::AbilityInputTagPressed(FGameplayTag InputTag)
{
	if (InputTag.MatchesTagExact(FTopDownRPGGameplayTags::Get().InputTag_LMB))
	{
		bTargeting = ThisActor ? true : false;
		bAutoRunning = false;
	}
}

void ATopDownRPGPlayerController::AbilityInputTagReleased(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FTopDownRPGGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagReleased(InputTag);
		}
		return;
	}

	if (GetASC())
	{
		GetASC()->AbilityInputTagReleased(InputTag);
	}

	if (!bTargeting && !bShiftKeyDown)
	{
		const APawn* ControlledPawn = GetPawn();
		if (FollowTime <= ShortPressThreshold && ControlledPawn)
		{
			if (UNavigationPath* NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, ControlledPawn->GetActorLocation(), CachedDestination))
			{
				Spline->ClearSplinePoints();
				for (const FVector& PointLoc : NavPath->PathPoints)
				{
					Spline->AddSplinePoint(PointLoc, ESplineCoordinateSpace::World);
				}
				if (NavPath->PathPoints.Num() > 0)
				{
					CachedDestination = NavPath->PathPoints.Last();
					bAutoRunning = true;
				}

			}
		}
		FollowTime = 0.f;
		bTargeting = false;
	}
}

void ATopDownRPGPlayerController::AbilityInputTagHeld(FGameplayTag InputTag)
{
	if (!InputTag.MatchesTagExact(FTopDownRPGGameplayTags::Get().InputTag_LMB))
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);
		}
		return;
	}
	if (bTargeting || bShiftKeyDown)
	{
		if (GetASC())
		{
			GetASC()->AbilityInputTagHeld(InputTag);
		}
	}
	else
	{
		FollowTime += GetWorld()->GetDeltaSeconds();

		if (CursorHit.bBlockingHit)
		{
			CachedDestination = CursorHit.ImpactPoint;
		}

		if (APawn* ControlledPawn = GetPawn())
		{
			const FVector WorldDirection = (CachedDestination - ControlledPawn->GetActorLocation()).GetSafeNormal();
			ControlledPawn->AddMovementInput(WorldDirection);
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

	UTopDownRPGInputComponent* TopDownRPGInputComponent = CastChecked<UTopDownRPGInputComponent>(InputComponent);

	TopDownRPGInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATopDownRPGPlayerController::Move);
	TopDownRPGInputComponent->BindAction(ShiftAction, ETriggerEvent::Started, this, &ATopDownRPGPlayerController::ShiftPressed);
	TopDownRPGInputComponent->BindAction(ShiftAction, ETriggerEvent::Completed, this, &ATopDownRPGPlayerController::ShiftReleased);

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

