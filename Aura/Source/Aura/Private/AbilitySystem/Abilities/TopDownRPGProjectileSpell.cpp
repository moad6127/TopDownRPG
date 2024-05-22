// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/TopDownRPGProjectileSpell.h"
#include "Actor/TopDownRPGProjectile.h"
#include "Interaction/CombatInterface.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "TopDownRPGGameplayTags.h"

void UTopDownRPGProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UTopDownRPGProjectileSpell::SpawnProjectile(const FVector& ProjectileTargetLocation, const FGameplayTag& SocektTag, bool bOverridePitch, float PitchOverride)
{
	
	const bool bIsServer = GetAvatarActorFromActorInfo()->HasAuthority();
	if (!bIsServer)
	{
		return;
	}
	ICombatInterface* CombatInterface = Cast<ICombatInterface>(GetAvatarActorFromActorInfo());

	const FVector SocketLocation = ICombatInterface::Execute_GetCombatSocketLocation(
		GetAvatarActorFromActorInfo(),
		SocektTag);
	FRotator Rotation = (ProjectileTargetLocation - SocketLocation).Rotation();
	if (bOverridePitch)
	{
		Rotation.Pitch = PitchOverride;

	}
	FTransform SpawnTrasnform;
	SpawnTrasnform.SetLocation(SocketLocation);
	SpawnTrasnform.SetRotation(Rotation.Quaternion());


	ATopDownRPGProjectile* Projectile = GetWorld()->SpawnActorDeferred<ATopDownRPGProjectile>(
		ProjectileClass,
		SpawnTrasnform,
		GetOwningActorFromActorInfo(), //Owner
		Cast<APawn>(GetOwningActorFromActorInfo()), //Instigator
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);


	const UAbilitySystemComponent* SourceASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo());
	FGameplayEffectContextHandle EffectContexthandle = SourceASC->MakeEffectContext();
	EffectContexthandle.SetAbility(this);
	EffectContexthandle.AddSourceObject(Projectile);
	TArray<TWeakObjectPtr<AActor>> Actors;
	Actors.Add(Projectile);
	EffectContexthandle.AddActors(Actors);
	FHitResult HitResult;
	HitResult.Location = ProjectileTargetLocation;
	EffectContexthandle.AddHitResult(HitResult);

	const FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), EffectContexthandle);
	const FTopDownRPGGameplayTags GameplayTags = FTopDownRPGGameplayTags::Get();
	for (auto& Pair : DamageTypes)
	{
		const float ScaledDamage = Pair.Value.GetValueAtLevel(GetAbilityLevel());
		UAbilitySystemBlueprintLibrary::AssignTagSetByCallerMagnitude(SpecHandle, Pair.Key, ScaledDamage);
	}

	Projectile->DamageEffectSpecHandle = SpecHandle;

	Projectile->FinishSpawning(SpawnTrasnform);
}

FString UTopDownRPGProjectileSpell::GetDescription(int32 Level)
{

	float Damage = 0.f;
	for (auto& Pair : DamageTypes)
	{
		Damage += Pair.Value.GetValueAtLevel(Level);
	}
	const int32 IntDamage = FMath::RoundToInt(Damage);

	if (Level == 1)
	{
		return FString::Printf(TEXT("<Title>FIRE BOLT</>\n\n<Default>Launches a bolt of fire, exploding on impact and dealing: </><Damage>%d</><Default> fire damage with a chance to burn</>\n\n<Small>Level: </><Level>%d</>"), IntDamage, Level);
	}

	return FString::Printf(TEXT("<Title>FIRE BOLT</>\n\n<Default>Launches %d bolts of fire, exploding on impact and dealing: </><Damage>%d</><Default> fire damage with a chance to burn</>\n\n<Small>Level: </><Level>%d</>"), FMath::Min(Level, NumProjectile), IntDamage, Level);
}

FString UTopDownRPGProjectileSpell::GetNextLevelDescription(int32 Level)
{
	float Damage = 0.f;
	for (auto& Pair : DamageTypes)
	{
		Damage += Pair.Value.GetValueAtLevel(Level);
	}
	const int32 IntDamage = FMath::RoundToInt(Damage);
	return FString::Printf(TEXT("<Title>NEXT LEVEL: </>\n\n<Default>Launches %d bolts of fire, exploding on impact and dealing: </><Damage>%d</><Default> fire damage with a chance to burn</>\n\n<Small>Level: </><Level>%d</>"), FMath::Min(Level, NumProjectile), IntDamage, Level);
}
