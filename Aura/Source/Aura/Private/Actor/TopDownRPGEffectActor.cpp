// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/TopDownRPGEffectActor.h"

#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/TopDownRPGAttributeSet.h"
#include "Components/SphereComponent.h"

ATopDownRPGEffectActor::ATopDownRPGEffectActor()
{
	PrimaryActorTick.bCanEverTick = false;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(GetRootComponent());
}

void ATopDownRPGEffectActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndx, bool bFromSweep, const FHitResult& SweepResult)
{
	if (IAbilitySystemInterface* ASCInterface = Cast<IAbilitySystemInterface>(OtherActor))
	{
		const UTopDownRPGAttributeSet* RPGAttributeSet = Cast<UTopDownRPGAttributeSet>(ASCInterface->GetAbilitySystemComponent()->GetAttributeSet(UTopDownRPGAttributeSet::StaticClass()));
		
		UTopDownRPGAttributeSet* MutableRPGAttributeSet = const_cast<UTopDownRPGAttributeSet*>(RPGAttributeSet);
		MutableRPGAttributeSet->SetHealth(RPGAttributeSet->GetHealth() + 25.f);

		Destroy();
	}
}

void ATopDownRPGEffectActor::EndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void ATopDownRPGEffectActor::BeginPlay()
{
	Super::BeginPlay();
	
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ATopDownRPGEffectActor::OnOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &ATopDownRPGEffectActor::EndOverlap);
}


