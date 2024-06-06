// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/TopDownRPGBeamSpell.h"
#include "GameFramework/Character.h"

void UTopDownRPGBeamSpell::StoreMouseDataInfo(const FHitResult& HitResult)
{
	if (HitResult.bBlockingHit)
	{
		MouseHitLocation = HitResult.ImpactPoint;
		MouseHitActor = HitResult.GetActor();
	}
	else
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
	}
}

void UTopDownRPGBeamSpell::StoreOwnerVariables()
{
	
	if (CurrentActorInfo)
	{
		OwnerPlayerController = CurrentActorInfo->PlayerController.Get();
		OwnerCharacter = Cast<ACharacter>(CurrentActorInfo->AvatarActor);
	}

}
