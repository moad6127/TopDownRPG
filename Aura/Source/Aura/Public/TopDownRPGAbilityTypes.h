#pragma once

#include "GameplayEffectTypes.h"
#include "TopDownRPGAbilityTypes.generated.h"

class UGameplayEffect;

USTRUCT(BlueprintType)
struct FDamageEffectParams
{
	GENERATED_BODY()

	FDamageEffectParams() {}

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UObject> WorldContextObject = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TSubclassOf<UGameplayEffect> DamageGamelayEffectClass = nullptr;

	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> SourceAbilitySystemComponent;
	
	UPROPERTY(BlueprintReadWrite)
	TObjectPtr<UAbilitySystemComponent> TargetAbilitySystemComponent;

	UPROPERTY(BlueprintReadWrite)
	float BaseDamage = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float AbilityLevel = 1.f;

	UPROPERTY(BlueprintReadWrite)
	FGameplayTag DamageType = FGameplayTag();

	UPROPERTY(BlueprintReadWrite)
	float DebuffChance = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float DebuffDamage = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float DebuffFrequency = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float DebuffDuration = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float DeathImpulseMagnitude = 0.f;

	UPROPERTY(BlueprintReadWrite)
	FVector DeathImpulse = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite)
	float KnockbackForceMagnitude = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float KnockbackChance = 0.f;

	UPROPERTY(BlueprintReadWrite)
	FVector KnockbackForce = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite)
	bool bisRadialDamage = false;

	UPROPERTY(BlueprintReadWrite)
	float RadialDamageInnerRadius = 0.f;

	UPROPERTY(BlueprintReadWrite)
	float RadialDamageOuterRadius = 0.f;

	UPROPERTY(BlueprintReadWrite)
	FVector RadialDamageOrigin = FVector::ZeroVector;

};

USTRUCT(BlueprintType)
struct FTopDownRPGGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:

	//~ getter
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
	FVector GetDeathImpulse() const
	{
		return DeathImpulse;
	}
	FVector GetKnockBackForce() const
	{
		return KnockbackForce;
	}
	bool IsRadialDamage() const
	{
		return bisRadialDamage;
	}
	float GetRadialDamageInnerRadius() const
	{
		return RadialDamageInnerRadius;
	}
	float GetRadialDamageOuterRadius() const
	{
		return RadialDamageOuterRadius;
	}
	FVector GetRadialDamageOrigin() const
	{
		return RadialDamageOrigin;
	}

	//~setter
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
	void SetDamageType(TSharedPtr<FGameplayTag> InDamageType)
	{
		DamageType = InDamageType;
	}
	void SetDeathImpulse(const FVector& InImpulse)
	{ 
		DeathImpulse = InImpulse;
	}
	void SetKnockbackForce(const FVector& InForce)
	{
		KnockbackForce = InForce;
	}
	void SetIsRadialDamage(bool bInIsRadialDamage)
	{
		bisRadialDamage = bInIsRadialDamage;
	}
	void SetRadialDamageInnerRadius(float InRadialDamageInnerRadius)
	{
		RadialDamageInnerRadius = InRadialDamageInnerRadius;
	}
	void SetRadialDamageOuterRadius(float InRadialDamageOutterRadius)
	{
		RadialDamageOuterRadius = InRadialDamageOutterRadius;
	}
	void SetRadialDamageOrigin(const FVector& InRadialDamageOrigin)
	{
		RadialDamageOrigin = InRadialDamageOrigin;
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

	UPROPERTY()
	FVector DeathImpulse = FVector::ZeroVector;

	UPROPERTY()
	FVector KnockbackForce = FVector::ZeroVector;

	UPROPERTY()
	bool bisRadialDamage = false;

	UPROPERTY()
	float RadialDamageInnerRadius = 0.f;

	UPROPERTY()
	float RadialDamageOuterRadius = 0.f;

	UPROPERTY()
	FVector RadialDamageOrigin = FVector::ZeroVector;

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