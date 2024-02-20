

#include "Character/TopDownRPGCharacterBase.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/TopDownRPGAbilitySystemComponent.h"

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

void ATopDownRPGCharacterBase::ApplyEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level) const
{
	check(IsValid(GetAbilitySystemComponent()));
	check(GameplayEffectClass);
	FGameplayEffectContextHandle ContextHandle = GetAbilitySystemComponent()->MakeEffectContext();
	ContextHandle.AddSourceObject(this);
	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(GameplayEffectClass, Level, ContextHandle);
	GetAbilitySystemComponent()->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), GetAbilitySystemComponent());
}

void ATopDownRPGCharacterBase::InitializeDefaultAttributes() const
{
	ApplyEffectToSelf(DefaultPrimaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultSecondaryAttributes, 1.f);
	ApplyEffectToSelf(DefaultVitalAttributes, 1.f);
}

void ATopDownRPGCharacterBase::AddCharacterAbilities()
{
	UTopDownRPGAbilitySystemComponent* TopDownASC = CastChecked<UTopDownRPGAbilitySystemComponent>(AbilitySystemComponent);
	if (!HasAuthority())
	{
		return;
	}
	TopDownASC->AddCharacterAbilities(StartupAbilities);
}


