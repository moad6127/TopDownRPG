#pragma once

#include "GameplayEffectTypes.h"
#include "TopDownRPGAbilityTypes.generated.h"

class UGameplayEffect;

USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()

	FDamageEffectParams() {}

	UPROPERTY()
	TObjectPtr<UObject> WorldContextObject = nullptr;

	UPROPERTY()
	TSubclassOf<UGameplayEffect> DamageGamelayEffectClass = nullptr;

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent;
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent;

	UPROPERTY()
	float BaseDamage = 0.f;

	UPROPERTY()
	float AbilityLevel = 1.f;

	UPROPERTY()
	FGameplayTag DamageType = FGameplayTag();

	UPROPERTY()
	float DebuffChance = 0.f;

	UPROPERTY()
	float DebuffDamage = 0.f;

	UPROPERTY()
	float DebuffFrequency = 0.f;

	UPROPERTY()
	float DebuffDuration = 0.f;
};

USTRUCT(BlueprintType)
struct FTopDownRPGGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:

	bool IsCriticalHit() const 
	{ 
		return bIsCriticalHit;
	}
	bool IsBlockedHit() const 
	{ 
		return bIsBlockedHit;
	}
	bool IsSuccessfulDebuff() const
	{
		return bIsSuccessfulDebuff;
	}
	float GetDebuffDamage() const
	{
		return DebuffDamage;
	}
	float GetDebuffDuration() const
	{
		return DebuffDuration;
	}
	float GetDebuffFrequency() const
	{
		return DebuffFrequency;
	}
	TSharedPtr<FGameplayTag> GetDamageType() const
	{
		return DamageType;
	}

	void SetIsCriticalHit(bool bInIsCriticalHit)
	{
		bIsCriticalHit = bInIsCriticalHit;
	}
	void SetIsBlockedHit(bool bInIsBlockedHit)
	{
		bIsBlockedHit = bInIsBlockedHit;
	}
	void SetIsSuccessfulDebuff(bool bInIsDebuff)
	{
		bIsSuccessfulDebuff = bInIsDebuff;
	}
	void SetDebuffDamage(float InDebuffDamage)
	{
		DebuffDamage = InDebuffDamage;
	}
	void SetDebuffDuration(float InDebuffDuration)
	{
		DebuffDuration = InDebuffDuration;
	}
	void SetDebuffFrequency(float InDebuffFrequency)
	{
		DebuffFrequency = InDebuffFrequency;
	}

	/** Returns the actual struct used for serialization, subclasses must override this! */
	virtual UScriptStruct* GetScriptStruct() const
	{
		return StaticStruct();
	}

	/** Creates a copy of this context, used to duplicate for later modifications */
	virtual FTopDownRPGGameplayEffectContext* Duplicate() const
	{
		FTopDownRPGGameplayEffectContext* NewContext = new FTopDownRPGGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}

	/** Custom serialization, subclasses must override this */
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);

protected:

	UPROPERTY()
	bool bIsBlockedHit = false;

	UPROPERTY()
	bool bIsCriticalHit = false;

	UPROPERTY()
	bool bIsSuccessfulDebuff = false;

	UPROPERTY()
	float DebuffDamage = 0.f;

	UPROPERTY()
	float DebuffDuration = 0.f;

	UPROPERTY()
	float DebuffFrequency = 0.f;

	TSharedPtr<FGameplayTag> DamageType;
};

template<>
struct TStructOpsTypeTraits<FTopDownRPGGameplayEffectContext> : public TStructOpsTypeTraitsBase2<FTopDownRPGGameplayEffectContext>
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true		// Necessary so that TSharedPtr<FHitResult> Data is copied around
	};
};