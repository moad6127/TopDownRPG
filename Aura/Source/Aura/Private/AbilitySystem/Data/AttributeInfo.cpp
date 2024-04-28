// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Data/AttributeInfo.h"
#include "Aura/TopDownRPGLogChannels.h"

FTopDownRPGAttributeInfo UAttributeInfo::FindAttributeInfoForTag(const FGameplayTag& AttributeTag, bool bLogNotFound) const
{
	for (const FTopDownRPGAttributeInfo& Info : AttributeInformation)
	{
		if (Info.AttributeTag.MatchesTagExact(AttributeTag))
		{
			return Info;
		}
	}

	if (bLogNotFound)
	{
		UE_LOG(LogTopDownRPG, Error, TEXT("Can't find Info for AttributeTag [%s] On AttributeInfo [%s]."), *AttributeTag.ToString(), *GetNameSafe(this));
	}

	return FTopDownRPGAttributeInfo();
}
