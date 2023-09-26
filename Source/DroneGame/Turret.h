// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Pawn.h"
#include "Turret.generated.h"

UCLASS()
class DRONEGAME_API ATurret : public APawn
{
	GENERATED_BODY()

	UPROPERTY(Category=Turret, EditAnywhere)
	TObjectPtr<USphereComponent> SphereComponent;
	
	UPROPERTY(Category=Turret, EditAnywhere)
	TObjectPtr<UStaticMeshComponent> BaseMesh;

	UPROPERTY(Category=Turret, EditAnywhere)
	TObjectPtr<UStaticMeshComponent> GunMesh;

	UPROPERTY(Category=Sensor, EditAnywhere)
	TObjectPtr<class UPawnSensingComponent> PawnSensor;

	UPROPERTY(Category=Turret, EditAnywhere)
	int Damage =10;

	UPROPERTY(Category=Turret, EditAnywhere)
	int Health =10;

	UPROPERTY(Category=Turret, EditAnywhere)
	float RotationSpeed = 20.0f;

	UPROPERTY(Category=Turret, EditAnywhere)
	float PitchRotationMax = 20.0f;

	UPROPERTY(Category=Turret, EditAnywhere)
	float PitchRotationMin= -20.0f;

	UPROPERTY(Category=Turret, EditAnywhere)
	float FireDelay= 0.1f;
	
	UPROPERTY(EditAnywhere)
	FVector MuzzleOffset;

	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class ADroneGameProjectile> ProjectileClass;

public:
	ATurret();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSeePawn(APawn *OtherPawn);

public:	
	virtual void Tick(float DeltaTime) override;

    UFUNCTION()
	void Fire();

	void ApplyDamage(int Damage);
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
