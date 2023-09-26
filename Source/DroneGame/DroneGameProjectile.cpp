// Copyright Epic Games, Inc. All Rights Reserved.

#include "DroneGameProjectile.h"

#include "DroneCharacter.h"
#include "Turret.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

ADroneGameProjectile::ADroneGameProjectile() 
{
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &ADroneGameProjectile::OnHit);		

	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	RootComponent = CollisionComp;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;

	InitialLifeSpan = 3.0f;
}

void ADroneGameProjectile::SetDamage(int Value)
{
	Damage=Value;
}

void ADroneGameProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		
		if(ADroneCharacter* Drone = Cast<ADroneCharacter>(OtherActor))
		{
			Drone->ApplyDamage(Damage);
		}
		else if(ATurret* Turret = Cast<ATurret>(OtherActor))
		{
			Turret->ApplyDamage(Damage);
		}

		Destroy();
	}
}
