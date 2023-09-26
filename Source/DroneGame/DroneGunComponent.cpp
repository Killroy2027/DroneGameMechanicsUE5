// Fill out your copyright notice in the Description page of Project Settings.


#include "DroneGunComponent.h"

#include "DroneCharacter.h"
#include "DroneGameProjectile.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"

UDroneGunComponent::UDroneGunComponent()
{
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
	CurrentAmmo = MaxAmmo;
}

void UDroneGunComponent::SetupInput()
{
	if (APlayerController* PlayerController = Cast<APlayerController>(GetOwner()->GetInstigatorController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}

		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerController->InputComponent))
		{
			EnhancedInputComponent->BindAction(FireAction, ETriggerEvent::Triggered, this, &UDroneGunComponent::Fire);
		}
	}
}


void UDroneGunComponent::Fire()
{

	if(CurrentAmmo > 0)
	{
		if (ProjectileClass != nullptr)
		{
			UWorld* const World = GetWorld();
			if (World != nullptr)
			{
				APlayerController* PlayerController = Cast<APlayerController>(GetOwner()->GetInstigatorController());
				const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();

				const FVector SpawnLocation = GetComponentLocation() + SpawnRotation.RotateVector(MuzzleOffset);
	
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	
				ADroneGameProjectile* Bullet = World->SpawnActor<ADroneGameProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
				Bullet->SetDamage(Damage);

				CurrentAmmo--;
			}
		}
	}
}

void UDroneGunComponent::Reload()
{
	CurrentAmmo = MaxAmmo;
}
