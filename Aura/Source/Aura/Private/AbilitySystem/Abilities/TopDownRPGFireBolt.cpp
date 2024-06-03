// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/TopDownRPGFireBolt.h"
#include "TopDownRPGGameplayTags.h"
#include "Kismet/KismetSystemLibrary.h"

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
	const FVector LeftOfSpread = Forward.RotateAngleAxis(-ProjectileSpread / 2.f, FVector::UpVector);
	const FVector RightOfSpread = Forward.RotateAngleAxis(ProjectileSpread / 2.f, FVector::UpVector);

	//NumProjectile = FMath::Min(MaxNumProjectiles, GetAbilityLevel());


	if (NumProjectile > 1)
	{
		const float DeltaSpread = ProjectileSpread / (NumProjectile - 1);
		for (int32 i = 0; i < NumProjectile; i++)
		{
			const FVector Direction = LeftOfSpread.RotateAngleAxis(DeltaSpread * i, FVector::UpVector);
			UKismetSystemLibrary::DrawDebugArrow(
				GetAvatarActorFromActorInfo(),
				SocketLocation + FVector(0,0,10),
				SocketLocation + FVector(0, 0, 10) + Direction * 75.f,
				1,
				FLinearColor::Red,
				120.f,
				1.f);
		}
	}
	else
	{
		//Single Projectile
		UKismetSystemLibrary::DrawDebugArrow(
			GetAvatarActorFromActorInfo(),
			SocketLocation + FVector(0, 0, 10),
			SocketLocation + FVector(0, 0, 10) + Forward * 75.f,
			1,
			FLinearColor::Red,
			120.f,
			1.f);
	}

	UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation, SocketLocation + RightOfSpread * 100.f, 1, FLinearColor::White, 120.f, 1.f);
	UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation, SocketLocation + Forward * 100.f,1, FLinearColor::White, 120.f, 1.f);
	UKismetSystemLibrary::DrawDebugArrow(GetAvatarActorFromActorInfo(), SocketLocation, SocketLocation + LeftOfSpread * 100.f, 1, FLinearColor::Gray, 120.f, 1.f);

}
