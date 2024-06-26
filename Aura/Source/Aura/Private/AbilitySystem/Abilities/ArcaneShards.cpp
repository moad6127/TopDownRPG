// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/ArcaneShards.h"

FString UArcaneShards::GetDescription(int32 Level)
{
	float ScaledDamage = Damage.GetValueAtLevel(Level);
	const int32 IntDamage = FMath::RoundToInt(ScaledDamage);
	const float ManaCost = FMath::Abs(GetManaCost(Level));
	const float Cooldown = GetCooldown(Level);
	if (Level == 1)
	{
		return FString::Printf(TEXT(
			//Title
			"<Title>ARACNAE SHARDS</>\n\n"

			//Level
			"<Small>Level: </><Level>%d</>\n"
			//ManaCost
			"<Small>ManaCost: </><ManaCost>%.1f</>\n"
			//Cooldown
			"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

			//Description
			"<Default>Spawn a Aracne Shards,"
			"causing radial arcnae damage of </>"

			//Damage
			"<Damage>%d</><Default> at the shard origin."
			"</>\n\n"),

			//Values
			Level, ManaCost, Cooldown, IntDamage);
	}

	return FString::Printf(TEXT(
		//Title
		"<Title>ARACNAE SHARDS</>\n\n"

		//Level
		"<Small>Level: </><Level>%d</>\n"
		//ManaCost
		"<Small>ManaCost: </><ManaCost>%.1f</>\n"
		//Cooldown
		"<Small>Cooldown: </><Cooldown>%.1f</>\n\n"

		//Number of Arcnae
		"<Default>Spawn %d Aracne Shards,"
		"causing radial arcnae damage of </>"

		//Damage
		"<Damage>%d</><Default> at the shard origin."
		"</>\n\n"),

		//Values
		Level, ManaCost, Cooldown, FMath::Min(Level, MaxNumShards), IntDamage);
}

FString UArcaneShards::GetNextLevelDescription(int32 Level)
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

		//Number of Arcnae
		"<Default>Spawn %d Aracne Shards,"
		"causing radial arcnae damage of </>"

		//Damage
		"<Damage>%d</><Default> at the shard origin."
		"</>\n\n"),

		//Values
		Level, ManaCost, Cooldown, FMath::Min(Level, MaxNumShards), IntDamage);
}
