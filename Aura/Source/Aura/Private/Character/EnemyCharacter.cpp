// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EnemyCharacter.h"

void AEnemyCharacter::HighlightActor()
{
	bHighlight = true;
}

void AEnemyCharacter::UnHighlightActor()
{
	bHighlight = false;
}
