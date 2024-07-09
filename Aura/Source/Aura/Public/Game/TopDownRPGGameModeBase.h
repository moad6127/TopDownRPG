// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TopDownRPGGameModeBase.generated.h"

class UAbilityInfo;
class UCharacterClassInfo;
class UMVVM_LoadSlot;
class USaveGame;
class ULoadScreenSaveGame;
/**
 * 
 */
UCLASS()
class AURA_API ATopDownRPGGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category = "Characgter Class Defaults")
	TObjectPtr<UCharacterClassInfo> CharacterClassInfo;

	UPROPERTY(EditDefaultsOnly, Category = "Abiltiy Info")
	TObjectPtr<UAbilityInfo> AbilityInfo;

	void SaveSlotData(UMVVM_LoadSlot* LoadSlot, int32 SlotIndex);
	ULoadScreenSaveGame* GetSaveSlotData(const FString& SlotName, int32 SlotIndex) const;

	static void DeleteSlot(const FString& SlotName, int32 SlotIndex);

	void TravelToMap(UMVVM_LoadSlot* Slot);

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<USaveGame> LoadScreenSaveGameClass;

	UPROPERTY(EditDefaultsOnly)
	FString DefaultMapName;

	UPROPERTY(EditDefaultsOnly)
	TSoftObjectPtr<UWorld> DefaultMap;

	UPROPERTY(EditDefaultsOnly)
	TMap<FString, TSoftObjectPtr<UWorld>> Maps;

	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

protected:

	virtual void BeginPlay() override;
};
