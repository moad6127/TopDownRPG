// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/Electrocute.h"

FString UElectrocute::GetDescription(int32 Level)
{

	float ScaledDamage = Damage.GetValueAtLevel(Level);
	const int32 IntDamage = FMath::RoundToInt(ScaledDamage);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	if (Level == 1)
	{
		return FString::Printf(TEXT(
			//Title
			"<Title>ELECTROCUTE</>\n\n"

			//Level
			"<Small>Level: </><Level>%d</>\n"
			//ManaCost
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			//Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			//Description
			"<Default>Emits a Beam of lightning,"
			"connecting with the target, repeatdly causing </>"

			//Damage
			"<Damage>%d</><Default> lightning damage with"
			"a chance to stun</>\n\n"),

			//Values
			Level, ManaCost, Cooldown, IntDamage);
	}

	return FString::Printf(TEXT(
		//Title
		"<Title>ELECTROCUTE</>\n\n"

		//Level
		"<Small>Level: </><Level>%d</>\n"
		//ManaCost
		"<Small>ManaCost: </><ManaCost>%.1f</>\n"
		//Cooldown
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

		//Additional Number of Shock Target
		"<Default>Emits a Beam of lightning,"
		"propagating to %d additional targets nearby, causing </>"

		//Damage
		"<Damage>%d</><Default> lightning damage with"
		"a chance to stun</>\n\n"),

		//Values
		Level, ManaCost, Cooldown, FMath::Min(Level, MaxNumShockTargets - 1), IntDamage);
}

FString UElectrocute::GetNextLevelDescription(int32 Level)
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


		//Additional Number of Shock Target
		"<Default>Emits a Beam of lightning,"
		"propagating to %d additional targets nearby, causing </>"

		//Damage
		"<Damage>%d</><Default> lightning damage with"
		"a chance to stun</>\n\n"),

		//Values
		Level, ManaCost, Cooldown, FMath::Min(Level, MaxNumShockTargets - 1), IntDamage);
}
