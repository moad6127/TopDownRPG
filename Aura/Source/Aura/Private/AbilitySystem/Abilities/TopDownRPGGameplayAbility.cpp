// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/TopDownRPGGameplayAbility.h"
#include "AbilitySystem/TopDownRPGAttributeSet.h"

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

float UTopDownRPGGameplayAbility::GetManaCost(float InLevel) const
{
    float ManaCost = 0.f;
    if (const UGameplayEffect* CostEffect = GetCostGameplayEffect())
    {
        for (FGameplayModifierInfo Mod : CostEffect->Modifiers)
        {
            if (Mod.Attribute == UTopDownRPGAttributeSet::GetManaAttribute())
            {
                Mod.ModifierMagnitude.GetStaticMagnitudeIfPossible(InLevel, ManaCost);
                break;
            }
        }
    }
    return ManaCost;
}

float UTopDownRPGGameplayAbility::GetCooldown(float InLevel) const
{
    float Cooldown = 0.f;
    if (const UGameplayEffect* CooldownEffect = GetCooldownGameplayEffect())
    {
        CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(InLevel, Cooldown);
    }
    return Cooldown;
}
