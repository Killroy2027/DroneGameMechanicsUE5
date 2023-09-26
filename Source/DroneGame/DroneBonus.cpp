// Fill out your copyright notice in the Description page of Project Settings.


#include "DroneBonus.h"

#include "DroneCharacter.h"
#include "Components/BoxComponent.h"


ADroneBonus::ADroneBonus()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Box"));
	BoxComponent->SetCollisionProfileName(TEXT("Trigger"));
	RootComponent = BoxComponent;

	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ADroneBonus::OnOverlapBegin);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(BoxComponent);
}

void ADroneBonus::BeginPlay()
{
	Super::BeginPlay();
	
}

void ADroneBonus::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if(OtherActor && (OtherActor != this) && OtherComp)
	{
		if(ADroneCharacter* Drone = Cast<ADroneCharacter>(OtherActor))
		{
			if(Drone->GetCurrentHealth() < Drone->GetMaxHealth() || Drone->GetGunCurrentAmmo() < Drone->GetGunMaxAmmo())
			{
				Drone->Heal();
				Drone->ReloadGun();

				this->Destroy();
			}
		}
	}
}

void ADroneBonus::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

