// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTS_Project/RTSFPS/Shared/Upgrades/RTSUpgrade.h"
#include "RTS_Project/RTSFPS/GameSystems/HealthSystem/HealthComponent.h"
#include "HealthUpgrade.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UHealthUpgrade : public URTSUpgrade
{
	GENERATED_BODY()

protected:
	virtual UHealthComponent * TryGetHealthComp(const UObject * TestUpgrade) const;
	virtual void ApplyUpgrade(UObject * ToUpgrade) const override;


protected:
	UPROPERTY(EditDefaultsOnly)
	float MaxHealthIncrement = 10.0f;

};
