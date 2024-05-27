// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/TopDownRPGFireBolt.h"
#include "TopDownRPGGameplayTags.h"

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
