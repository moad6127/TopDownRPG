// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/TopDownRPGGameplayAbility.h"

FString UTopDownRPGGameplayAbility::GetDescription(int32 Level)
{
    return FString::Printf(TEXT("<Default>%s, </><Level>%d</>"), L"Default Ability Name - LoermIpsum LoermIpsum LoermIpsum LoermIpsum LoermIpsum LoermIpsum LoermIpsum LoermIpsum", Level);
}

FString UTopDownRPGGameplayAbility::GetNextLevelDescription(int32 Level)
{
    return FString::Printf(TEXT("<Default> Next Level: </><Level>%d</> \n<Default>Causes much more Damage </>"), Level);
}

FString UTopDownRPGGameplayAbility::GetLockedDecription(int32 Level)
{
    return FString::Printf(TEXT("<Default> Spell Locked Until Level: %d </>"), Level);
}
