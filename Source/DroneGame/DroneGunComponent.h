// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "DroneGunComponent.generated.h"

/**
 * 
 */
UCLASS()
class DRONEGAME_API UDroneGunComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = Ammo)
	int CurrentAmmo;

	UPROPERTY(EditAnywhere, Category = Ammo)
	int MaxAmmo = 10;

	UPROPERTY(VisibleAnywhere, Category = Damage)
	int Damage = 10;

	UPROPERTY(EditAnywhere)
	FVector MuzzleOffset;

	UPROPERTY(EditDefaultsOnly, Category=Projectile)
	TSubclassOf<class ADroneGameProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category=Input)
	class UInputAction* FireAction;

	UPROPERTY(EditAnywhere, Category=Input)
	class UInputMappingContext* FireMappingContext;

public:
	
	UDroneGunComponent();

	void SetupInput();

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void Fire();

	UFUNCTION(BlueprintCallable, Category="Weapon")
	void Reload();

	int GetCurrentAmmo() const {return CurrentAmmo; }

	int GetMaxAmmo() const {return MaxAmmo; }
};
