// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTS_Project/RTSFPS/Shared/Upgrades/RTSAbilityUpgrade.h"
#include "RTSAbilityEnableUpgrade.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API URTSAbilityEnableUpgrade : public URTSAbilityUpgrade
{
	GENERATED_BODY()
	virtual void ApplyUpgrade(UObject * ToUpgrade) const override;
};
