// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RTS_Project/RTSFPS/Shared/Upgrades/RTSAbilityUpgrade.h"
#include "MineResourceAbilityUpgrade.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UMineResourceAbilityUpgrade : public URTSAbilityUpgrade
{
	GENERATED_BODY()
	
public:
	virtual void ApplyUpgrade(UObject * ToUpgrade) const override;

protected:
	UPROPERTY(EditDefaultsOnly)
	float MineSpeedIncrement = 0.25f;

	UPROPERTY(EditDefaultsOnly)
	uint32 MineAmountIncrement = 10U;

};
