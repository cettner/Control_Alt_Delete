// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTS_Project/RTSFPS/Shared/Upgrades/RTSAbilityUpgrade.h"
#include "ProjectileAbilityUpgrade.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UProjectileAbilityUpgrade : public URTSAbilityUpgrade
{
	GENERATED_BODY()
public:
	virtual void ApplyUpgrade(UObject * ToUpgrade) const override;


protected:
	UPROPERTY(EditDefaultsOnly)
	float VelocityIncrement = 500.0f;

	UPROPERTY(EditDefaultsOnly)
	float DamageIncrement = 10.0f;
};
