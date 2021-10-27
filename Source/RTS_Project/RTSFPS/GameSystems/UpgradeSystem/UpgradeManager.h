// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Info.h"

#include "Interfaces/UpgradableInterface.h"
#include "Upgrade.h"
#include "UpgradeManager.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API AUpgradeManager : public AInfo
{
	GENERATED_BODY()
	public:
		AUpgradeManager();

		bool CheckAndDispatchUpgrade(TSubclassOf<UUpgrade> UpgradeClass, TArray<AActor*> &Applyto);

		UFUNCTION(NetMultiCast, reliable, WithValidation)
		void DispatchUpgrade(TSubclassOf<UUpgrade> UpgradeClass, const TArray<AActor*> &Applyto);

		TArray<UUpgrade *> ActiveUpgrades;

};
