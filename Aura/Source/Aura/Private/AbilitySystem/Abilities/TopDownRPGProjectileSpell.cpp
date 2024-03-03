// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/TopDownRPGProjectileSpell.h"
#include "Actor/TopDownRPGProjectile.h"
#include "Interaction/CombatInterface.h"


void UTopDownRPGProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);


}

void UTopDownRPGProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation)
{
	
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer)
	{
		return;
	}
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());

	if (CombatInterface)
	{
		const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();
		FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
		Rotation.Pitch = 0.f;

		FTransform SpawnTrasnform;
		SpawnTrasnform.SetLocation(SocketLocation);
		SpawnTrasnform.SetRotation(Rotation.Quaternion());


		ATopDownRPGProjectile* Projectile = GetWorld()->SpawnActorDeferred<ATopDownRPGProjectile>(
			ProjectileClass,
			SpawnTrasnform,
			GetOwningActorFromActorInfo(), //Owner
			Cast<APawn>(GetOwningActorFromActorInfo()), //Instigator
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);


		// TODO : Give the Projectile a Gameplay Effect spec for causing damage
		Projectile->FinishSpawning(SpawnTrasnform);
	}
}
