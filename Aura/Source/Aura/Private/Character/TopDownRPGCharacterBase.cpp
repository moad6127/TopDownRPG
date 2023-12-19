

#include "Character/TopDownRPGCharacterBase.h"

ATopDownRPGCharacterBase::ATopDownRPGCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ATopDownRPGCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

