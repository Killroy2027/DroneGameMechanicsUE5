// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DroneHud.generated.h"

/**
 * 
 */
UCLASS()
class DRONEGAME_API UDroneHud : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;

	virtual bool Initialize() override;
	
	UPROPERTY()
	TObjectPtr<class ADroneCharacter> DroneRef;

	UFUNCTION()
	float UpdateHealth();
	
	UFUNCTION()
	FText UpdateAmmo();
	
public:
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UProgressBar* HealthBar;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UTextBlock* AmmoText;
};
