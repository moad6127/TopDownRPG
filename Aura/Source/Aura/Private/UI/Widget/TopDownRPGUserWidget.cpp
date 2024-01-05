// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Widget/TopDownRPGUserWidget.h"

void UTopDownRPGUserWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	WidgetControllerSet();
}
