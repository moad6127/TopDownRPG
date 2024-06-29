// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/TopDownRPGFireBall.h"

void ATopDownRPGFireBall::BeginPlay()
{
	Super::BeginPlay();
	StartOutgoingTimeline();
}

void ATopDownRPGFireBall::OnSphereOverlap(UPrimitiveComponent* OverlappingComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}
