// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "InputMappingQuery.h"
#include "Camera/CameraComponent.h"
#include "Components/BoxComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/Pawn.h"
#include "DroneCharacter.generated.h"

UCLASS()
class DRONEGAME_API ADroneCharacter : public APawn
{
	GENERATED_BODY()

	UPROPERTY(Category=Drone, EditAnywhere)
	TObjectPtr<UFloatingPawnMovement> FloatingMovement;

	UPROPERTY(Category=Drone, EditAnywhere)
	TObjectPtr<UBoxComponent> BoxComponent;

	UPROPERTY(VisibleAnywhere,Category = Camera)
	TObjectPtr<UCameraComponent> FirstPersonCameraComponent;
	
	UPROPERTY(Category=Drone, EditAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(Category=Drone, EditAnywhere)
	TObjectPtr<class UDroneGunComponent> Gun;

	UPROPERTY(EditAnywhere, Category=Input)
	TObjectPtr<UInputMappingContext> DefaultMappingContext;

	UPROPERTY(EditAnywhere, Category=Input)
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category=Input)
	TObjectPtr<UInputAction> UpAction;
	
	UPROPERTY(EditAnywhere, Category = Input)
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(VisibleAnywhere, Category = Health)
	float CurrentHealth;

	UPROPERTY(EditAnywhere, Category = Health)
	float MaxHealth = 100.0f;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UDroneHud> DroneHUDClass;

	UPROPERTY()
	TObjectPtr<class UDroneHud> DroneHUD;

	UPROPERTY()
	bool bDead = false;
	
public:
	ADroneCharacter();
	
protected:
	virtual void BeginPlay() override;
	
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	void Move(const FInputActionValue& Value);

	void MoveUp(const FInputActionValue& Value);
	
	void Look(const FInputActionValue& Value);
	
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

public:	

	virtual void Tick(float DeltaTime) override;

	void Die();

	void Heal();
	
	void ApplyDamage(int Value);

	void ReloadGun();

	UStaticMeshComponent* GetMesh() const { return Mesh; }

	float GetCurrentHealth() const {return CurrentHealth; }
	
	float GetMaxHealth() const {return MaxHealth; }

	int GetGunCurrentAmmo() const;

	int GetGunMaxAmmo() const;
	
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
	
};
