// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/TopDownRPGFireBlast.h"
#include "Actor/TopDownRPGFireBall.h"
#include "AbilitySystem/TopDownRPGAbilitySystemLibrary.h"

FString UTopDownRPGFireBlast::GetDescription(int32 Level)
{
	float ScaledDamage = Damage.GetValueAtLevel(Level);
	const int32 IntDamage = FMath::RoundToInt(ScaledDamage);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);

	return FString::Printf(TEXT(
		//Title
		"<Title>FIRE BLAST</>\n\n"

		//Level
		"<Small>Level: </><Level>%d</>\n"
		//ManaCost
		"<Small>ManaCost: </><ManaCost>%.1f</>\n"
		//Cooldown
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

		//Number of Fire balls
		"<Default>Launches %d</>"
		"<Default>fire balls in all directions, each coming back and </>"
		"<Default>exploding upon return, causing </>"

		//Damage
		"<Damage>%d</><Default> radial fire damage with"
		"a chance to burn</>\n\n"),

		//Values
		Level, ManaCost, Cooldown, NumFireBalls, IntDamage);


}

FString UTopDownRPGFireBlast::GetNextLevelDescription(int32 Level)
{
	float ScaledDamage = Damage.GetValueAtLevel(Level);
	const int32 IntDamage = FMath::RoundToInt(ScaledDamage);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);

	return FString::Printf(TEXT(
		//Title
		"<Title>NEXT LEVEL:</>\n\n"

		//Level
		"<Small>Level: </><Level>%d</>\n"
		//ManaCost
		"<Small>ManaCost: </><ManaCost>%.1f</>\n"
		//Cooldown
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

		//Number of Fire balls
		"<Default>Launches %d</>"
		"<Default>fire balls in all directions, each coming back and </>"
		"<Default>exploding upon return, causing </>"

		//Damage
		"<Damage>%d</><Default> radial fire damage with"
		"a chance to burn</>\n\n"),

		//Values
		Level, ManaCost, Cooldown, NumFireBalls, IntDamage);
}

TArray<ATopDownRPGFireBall*> UTopDownRPGFireBlast::SpawnFireBalls()
{
	TArray<ATopDownRPGFireBall*> FireBalls;
	const FVector Forward = GetAvatarActorFromActorInfo()->GetActorForwardVector();
	const FVector Location = GetAvatarActorFromActorInfo()->GetActorLocation();
	TArray<FRotator> Rotators = UTopDownRPGAbilitySystemLibrary::EvenlySpacedRotators(Forward,FVector::UpVector,360.f,NumFireBalls);

	for (const FRotator& Rotator : Rotators)
	{
		FTransform SpawnTransform;
		SpawnTransform.SetLocation(Location);
		SpawnTransform.SetRotation(Rotator.Quaternion());
		ATopDownRPGFireBall* FireBall = GetWorld()->SpawnActorDeferred<ATopDownRPGFireBall>(
			FireBallClass,
			SpawnTransform,
			GetOwningActorFromActorInfo(),
			CurrentActorInfo->PlayerController->GetPawn(),
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

		FireBall->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();

		FireBalls.Add(FireBall);
		FireBall->FinishSpawning(SpawnTransform);
	}


	return FireBalls;
}
