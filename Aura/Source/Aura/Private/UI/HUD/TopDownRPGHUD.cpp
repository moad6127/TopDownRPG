// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/TopDownRPGHUD.h"
#include "UI/Widget/TopDownRPGUserWidget.h"
#include "UI/WidgetController/OverlayWidgetController.h"

UOverlayWidgetController* ATopDownRPGHUD::GetOverlayWidgetController(const FWidgetControllerParams& WCParams)
{
	if (OverlayWidgetController == nullptr)
	{
		OverlayWidgetController = NewObject<UOverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->SetWidgetControllerParams(WCParams);
		OverlayWidgetController->BindCallbacksToDependencies();

		return OverlayWidgetController;
	}
	return OverlayWidgetController;
}

void ATopDownRPGHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	checkf(OverlayWidgetClass, TEXT("Overlay Widget Class uninitialized, fill out BP_TopDownRPGHUD"));
	checkf(OverlayWidgetControllerClass, TEXT("Overlay Widget Controller Class uninitialized, fill out BP_TopDownRPGHUD"));

	OverlayWidget = CreateWidget<UTopDownRPGUserWidget>(GetWorld(), OverlayWidgetClass);


	const FWidgetControllerParams WidgetControllerParmas(PC,PS,ASC,AS);

	UOverlayWidgetController* WidgetController = GetOverlayWidgetController(WidgetControllerParmas);

	OverlayWidget->SetWidgetController(WidgetController);
	WidgetController->BroadcastInitialValue();

	OverlayWidget->AddToViewport();
}

