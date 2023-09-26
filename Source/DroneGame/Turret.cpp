// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"

#include "DroneGameProjectile.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Perception/PawnSensingComponent.h"

ATurret::ATurret()
{
	PrimaryActorTick.bCanEverTick = true;
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);

	
	BaseMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base Mesh"));
	RootComponent =	BaseMesh;
	
	
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	SphereComponent->SetupAttachment(BaseMesh);
	
	GunMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Gun Mesh"));
	GunMesh->SetupAttachment(BaseMesh);

	PawnSensor = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("Pawn Sensor"));
	PawnSensor->OnSeePawn.AddDynamic(this, &ATurret::OnSeePawn);
	
}

void ATurret::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATurret::OnSeePawn(APawn* OtherPawn)
{
	if(UWorld* World = GetWorld())
	{
		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), OtherPawn->GetActorLocation());
		FRotator InterpRotation = UKismetMathLibrary::RInterpTo(this->GetActorRotation(), LookAtRotation, World->GetDeltaSeconds(), RotationSpeed);
		
		float Pitch = UKismetMathLibrary::FClamp(InterpRotation.Pitch, PitchRotationMin, PitchRotationMax);
		
		this->SetActorRotation(FRotator(Pitch, InterpRotation.Yaw ,0));
		
		FLatentActionInfo Info;
        Info.Linkage = 0;
        Info.CallbackTarget = this;
        Info.ExecutionFunction = "Fire";
        Info.UUID = GetUniqueID();
        UKismetSystemLibrary::RetriggerableDelay(GetWorld(), FireDelay, Info);
        
	}
	
}

void ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATurret::Fire()
{
	if (ProjectileClass != nullptr)
	{
		UWorld* const World = GetWorld();
		if (World != nullptr)
		{
			const FRotator SpawnRotation = GunMesh->GetComponentRotation();

			const FVector SpawnLocation = GunMesh->GetComponentLocation() + SpawnRotation.RotateVector(MuzzleOffset);
	
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	
			ADroneGameProjectile* Bullet = World->SpawnActor<ADroneGameProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			Bullet->SetDamage(Damage);
			
		}
	}
}

void ATurret::ApplyDamage(int Value)
{
	Health -= Value;
	if(Health < 0) Destroy();
}

void ATurret::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

