// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "TopDownRPGHUD.generated.h"

/**
 * 
 */
class UAttributeMenuWidgetController;
class UTopDownRPGUserWidget;
class UOverlayWidgetController;
class UAbilitySystemComponent;
class UAttributeSet;

struct FWidgetControllerParams;
class USpellMenuWidgetController;

UCLASS()
class AURA_API ATopDownRPGHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	
	//~ Value

	//~ Value


	//~ Function
	UOverlayWidgetController* GetOverlayWidgetController(const FWidgetControllerParams& WCParams);
	UAttributeMenuWidgetController* GetAttributeMenuController(const FWidgetControllerParams& WCParams);
	USpellMenuWidgetController* GetSpellMenuController(const FWidgetControllerParams& WCParams);


	void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS);
	//~ Function

protected:

private:
	UPROPERTY()
	TObjectPtr<UTopDownRPGUserWidget> OverlayWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UTopDownRPGUserWidget> OverlayWidgetClass;

	UPROPERTY()
	TObjectPtr<UOverlayWidgetController> OverlayWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UOverlayWidgetController> OverlayWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<UAttributeMenuWidgetController> AttributeMenuWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAttributeMenuWidgetController> AttributeMenuWidgetControllerClass;

	UPROPERTY()
	TObjectPtr<USpellMenuWidgetController> SpellMenuWidgetController;

	UPROPERTY(EditAnywhere)
	TSubclassOf<USpellMenuWidgetController> SpellMenuWidgetControllerClass;
};
