// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/TopDownRPGFireBolt.h"
#include "TopDownRPGGameplayTags.h"
#include "AbilitySystem/TopDownRPGAbilitySystemLibrary.h"
#include "Actor/TopDownRPGProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"

FString UTopDownRPGFireBolt::GetDescription(int32 Level)
{

	float ScaledDamage = Damage.GetValueAtLevel(Level);
	const int32 IntDamage = FMath::RoundToInt(ScaledDamage);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	if (Level == 1)
	{
		return FString::Printf(TEXT(
			//Title
			"<Title>FIRE BOLT</>\n\n"

			//Level
			"<Small>Level: </><Level>%d</>\n"
			//ManaCost
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			//Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			//Description
			"<Default>Launches a bolt of fire,"
			" exploding on impact and dealing: </>"

			//Damage
			"<Damage>%d</><Default> "
			"fire damage with a chance to burn</>\n\n"),

			//Values
			Level, ManaCost, Cooldown, IntDamage);
	}

	return FString::Printf(TEXT(
		//Title
		"<Title>FIRE BOLT</>\n\n"

		//Level
		"<Small>Level: </><Level>%d</>\n"
		//ManaCost
		"<Small>ManaCost: </><ManaCost>%.1f</>\n"
		//Cooldown
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

		//Number of FireBolt
		"<Default>Launches %d bolt of fire,"
		" exploding on impact and dealing: </>"

		//Damage
		"<Damage>%d</><Default> "
		"fire damage with a chance to burn</>\n\n"),

		//Values
		Level, ManaCost, Cooldown,FMath::Min(Level,NumProjectile), IntDamage);
}

FString UTopDownRPGFireBolt::GetNextLevelDescription(int32 Level)
{
	float ScaledDamage = Damage.GetValueAtLevel(Level);
	const int32 IntDamage = FMath::RoundToInt(ScaledDamage);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	return FString::Printf(TEXT(
		//Title
		"<Title>NEXT LEVEL</>\n\n"

		//Level
		"<Small>Level: </><Level>%d</>\n"
		//ManaCost
		"<Small>ManaCost: </><ManaCost>%.1f</>\n"
		//Cooldown
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"


		//Number of FireBolt
		"<Default>Launches %d bolt of fire,"
		" exploding on impact and dealing: </>"

		//Damage
		"<Damage>%d</><Default> "
		"fire damage with a chance to burn</>\n\n"),

		//Values
		Level, ManaCost,Cooldown, FMath::Min(Level, NumProjectile), IntDamage);
}

void UTopDownRPGFireBolt::SpawnProjectiles(const FVector& ProjectileTargetLocation, const FGameplayTag& SocektTag, bool bOverridePitch, float PitchOverride, AActor* HomingTarget)
{
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer)
	{
		return;
	}

	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(
		GetAvatarActorFromActorInfo(),
		SocektTag);
	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();

	if (bOverridePitch)
	{
		Rotation.Pitch = PitchOverride;
	}
	
	const FVector Forward = Rotation.Vector();

	NumProjectile = FMath::Min(NumProjectile, GetAbilityLevel());
	
	TArray<FRotator> Rotations = UTopDownRPGAbilitySystemLibrary::EvenlySpacedRotators(Forward, FVector::UpVector, ProjectileSpread, NumProjectile);

	for (const FRotator& Rot : Rotations)
	{
		FTransform SpawnTrasnform;
		SpawnTrasnform.SetLocation(SocketLocation);
		SpawnTrasnform.SetRotation(Rot.Quaternion());


		ATopDownRPGProjectile* Projectile = GetWorld()->SpawnActorDeferred<ATopDownRPGProjectile>(
			ProjectileClass,
			SpawnTrasnform,
			GetOwningActorFromActorInfo(), //Owner
			Cast<APawn>(GetOwningActorFromActorInfo()), //Instigator
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();

		if (HomingTarget && HomingTarget->Implements<UCombatInterface>())
		{
			Projectile->ProjectileMovementComponent->HomingTargetComponent = HomingTarget->GetRootComponent();
		}
		else
		{
			Projectile->HomingTargetSceneComponent = NewObject<USceneComponent>(USceneComponent::StaticClass());
			Projectile->HomingTargetSceneComponent->SetWorldLocation(ProjectileTargetLocation);
			Projectile->ProjectileMovementComponent->HomingTargetComponent = Projectile->HomingTargetSceneComponent;
		}
		Projectile->ProjectileMovementComponent->HomingAccelerationMagnitude = FMath::FRandRange(HomingAccelerationMin, HomingAccelerationMax);
		Projectile->ProjectileMovementComponent->bIsHomingProjectile = bLaunchHomingProjectile;

		Projectile->FinishSpawning(SpawnTrasnform);


	}


}
