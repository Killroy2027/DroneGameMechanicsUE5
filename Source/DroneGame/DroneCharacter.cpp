// Fill out your copyright notice in the Description page of Project Settings.


#include "DroneCharacter.h"

#include "DroneGunComponent.h"
#include "DroneHud.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"
#include "Components/StaticMeshComponent.h"

ADroneCharacter::ADroneCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetCollisionProfileName(UCollisionProfile::BlockAll_ProfileName);
	BoxComponent->SetShouldUpdatePhysicsVolume(true);
	BoxComponent->bDynamicObstacle = true;
	
	RootComponent = BoxComponent;

	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(BoxComponent);
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DroneMesh"));
	Mesh->SetupAttachment(FirstPersonCameraComponent);

	FloatingMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));
	if (FloatingMovement)
	{
		FloatingMovement->UpdatedComponent = BoxComponent;
	}

	Gun = CreateDefaultSubobject<UDroneGunComponent>(TEXT("DroneGun"));
	Gun->SetupAttachment(FirstPersonCameraComponent);
	
	bUseControllerRotationYaw = true;
	CurrentHealth = MaxHealth;
	
}

void ADroneCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}

		if(DroneHUDClass)
		{
			DroneHUD = CreateWidget<UDroneHud>(PlayerController, DroneHUDClass);

			if(DroneHUD)
			{
				DroneHUD->AddToPlayerScreen();	
			}
		}
	}
}

void ADroneCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if(DroneHUD)
	{
		DroneHUD->RemoveFromParent();
		DroneHUD=nullptr;
	}

	Super::EndPlay(EndPlayReason);
}


void ADroneCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ADroneCharacter::MoveUp(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddMovementInput(GetActorUpVector(), MovementVector.X);
	}
}

void ADroneCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ADroneCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADroneCharacter::Die()
{
	DetachFromControllerPendingDestroy();
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);

	Gun->SetCollisionProfileName(TEXT("Ragdoll"));
	
	Mesh->SetCollisionProfileName(TEXT("Ragdoll"));
	
	SetActorEnableCollision(true);

	Gun->SetSimulatePhysics(true);
	Gun->WakeAllRigidBodies();
	
	Mesh->SetSimulatePhysics(true);
	Mesh->WakeAllRigidBodies();
	
	FloatingMovement->StopMovementImmediately();
	FloatingMovement->SetComponentTickEnabled(false);

	SetLifeSpan(10.0f);
	bDead = true;
}

void ADroneCharacter::Heal()
{
	CurrentHealth = MaxHealth;
}

void ADroneCharacter::ApplyDamage(int Value)
{
	if(!bDead)
	{
		CurrentHealth -= Value;
		if(CurrentHealth <= 0)
		{
			Die();
		}
	}
	
}

void ADroneCharacter::ReloadGun()
{
	Gun->Reload();
}

int ADroneCharacter::GetGunCurrentAmmo() const
{
	return Gun->GetCurrentAmmo();
}

int ADroneCharacter::GetGunMaxAmmo() const
{
	return Gun->GetMaxAmmo();
}

void ADroneCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if(Gun !=nullptr) Gun->SetupInput();
		
		EnhancedInputComponent->BindAction(UpAction, ETriggerEvent::Triggered, this, &ADroneCharacter::MoveUp);
		
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADroneCharacter::Move);

		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADroneCharacter::Look);
	}

}

