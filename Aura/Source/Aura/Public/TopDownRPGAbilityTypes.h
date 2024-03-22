#pragma once

#include "GameplayEffectTypes.h"
#include "TopDownRPGAbilityTypes.generated.h"

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

	void SetIsCriticalHit(bool bInIsCriticalHit)
	{
		bIsCriticalHit = bInIsCriticalHit;
	}
	void SetIsBlockedHit(bool bInIsBlockedHit)
	{
		bIsBlockedHit = bInIsBlockedHit;
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