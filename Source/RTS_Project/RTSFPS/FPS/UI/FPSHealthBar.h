// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTS_Project/RTSFPS/Shared/UI/MaterialProgressBar.h"
#include "RTS_Project/RTSFPS/GameSystems/HealthSystem/HealthComponent.h"
#include "FPSHealthBar.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UFPSHealthBar : public UMaterialProgressBar
{
	GENERATED_BODY()
	
	protected:

		virtual void NativeConstruct() override;

		UFUNCTION()
		virtual float UpdatePercentHealth();

		virtual UHealthComponent* GetOwnerHealthComp() const;
	
};
