// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUD/TopDownRPGHUD.h"
#include "UI/Widget/TopDownRPGUserWidget.h"

void ATopDownRPGHUD::BeginPlay()
{
	Super::BeginPlay();

	check(OverlayWidgetClass);
	UUserWidget* Widget = CreateWidget<UUserWidget>(GetWorld(), OverlayWidgetClass);

	check(Widget);
	Widget->AddToViewport();
}
