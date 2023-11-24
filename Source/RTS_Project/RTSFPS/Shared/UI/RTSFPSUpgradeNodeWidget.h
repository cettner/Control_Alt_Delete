// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTS_Project/RTSFPS/GameSystems/UpgradeSystem/UI/UpgradeNodeWidget.h"
#include "RTS_Project/RTSFPS/GameSystems/UpgradeSystem/Interfaces/ExpAccumulatorInterface.h"
#include "RTS_Project/RTSFPS/GameSystems/ResourceSystem/Interfaces/ResourceGatherer.h"
#include "RTSFPSUpgradeNodeWidget.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API URTSFPSUpgradeNodeWidget : public UUpgradeNodeWidget
{
	GENERATED_BODY()
	protected:
		virtual void ApplyUpgrade(IUpgradableInterface* UpgradeUser) const override;
		virtual void RemoveUpgrade(IUpgradableInterface* UpgradeUser) const override;
		virtual bool CanPurchaseUpgrade() const override;
	
	protected:
		virtual IExpAccumulatorInterface * GetExpSource() const;
};
