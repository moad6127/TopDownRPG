

#include "Character/TopDownRPGCharacterBase.h"

ATopDownRPGCharacterBase::ATopDownRPGCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>("Weapon");
	Weapon->SetupAttachment(GetMesh(), FName("WeaponHandSocket"));
	Weapon->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

UAbilitySystemComponent* ATopDownRPGCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ATopDownRPGCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATopDownRPGCharacterBase::InitAbilityActorInfo()
{
}

