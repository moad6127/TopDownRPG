// Fill out your copyright notice in the Description page of Project Settings.


#include "Checkpoint/MapEntrance.h"
#include "Interaction/PlayerInterface.h"
#include "Game/TopDownRPGGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"

AMapEntrance::AMapEntrance(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	Sphere->SetupAttachment(MoveToComp);
	UCapsuleComponent* CapComp = GetCapsuleComponent();
	CapComp->bDrawOnlyIfSelected = false;
}

void AMapEntrance::HighlightActor_Implementation()
{
	CheckpointMesh->SetRenderCustomDepth(true);
}

void AMapEntrance::LoadActor_Implementation()
{
	// Do noting when loading a Map Ent
}

void AMapEntrance::OnSphereOverlap(UPrimitiveComponent* OverlappingComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->Implements<UPlayerInterface>())
	{
		bReached = true;

		if (ATopDownRPGGameModeBase* GameMode = Cast<ATopDownRPGGameModeBase>(UGameplayStatics::GetGameMode(this)))
		{
			GameMode->SaveWorldState(GetWorld(), DestinationMap.ToSoftObjectPath().GetAssetName());
		}

		IPlayerInterface::Execute_SaveProgress(OtherActor, DestinationPlayerStartTag);

		UGameplayStatics::OpenLevelBySoftObjectPtr(this, DestinationMap);
	}
}
