// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTS_Project/RTSFPS/GameSystems/UpgradeSystem/Upgrade.h"
#include "RTS_Project/RTSFPS/BaseClasses/Interfaces/RTSObjectInterface.h"
#include "RTSUpgrade.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API URTSUpgrade : public UUpgrade, public IRTSObjectInterface
{
	GENERATED_BODY()
	
};
