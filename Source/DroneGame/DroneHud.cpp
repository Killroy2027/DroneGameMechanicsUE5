// Fill out your copyright notice in the Description page of Project Settings.


#include "DroneHud.h"

#include "DroneCharacter.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UDroneHud::NativeConstruct()
{
	Super::NativeConstruct();
}

bool UDroneHud::Initialize()
{
	bool Success = Super::Initialize();

	if(!Success) return false;
	
	DroneRef = Cast<ADroneCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
	
	if(HealthBar)
	{
		HealthBar->PercentDelegate.BindUFunction(this, "UpdateHealth");
	}

	if(AmmoText)
	{
		AmmoText->TextDelegate.BindUFunction(this, "UpdateAmmo");
	}

	return true;
}

float UDroneHud::UpdateHealth()
{
	if(DroneRef)
	{
		return DroneRef->GetCurrentHealth()/DroneRef->GetMaxHealth();
	}
	return 0;
}

FText UDroneHud::UpdateAmmo()
{
	if(DroneRef)
	{
		return FText::FromString("Ammo: " + FString::FromInt(DroneRef->GetGunCurrentAmmo()));
	}
	return  FText::FromString("NULL");
}
