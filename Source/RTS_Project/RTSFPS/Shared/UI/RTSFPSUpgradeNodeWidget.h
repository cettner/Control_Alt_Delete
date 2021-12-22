// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTS_Project/RTSFPS/GameSystems/UpgradeSystem/UI/UpgradeNodeWidget.h"
#include "RTS_Project/RTSFPS/Shared/Interfaces/ResourceGatherer.h"
#include "RTS_Project/RTSFPS/GameObjects/Resource.h"
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
		virtual bool CanPurchaseUpgrade() const override;
	
	protected:
		virtual FReplicationResourceMap GetUpgradeCost() const;
		virtual IResourceGatherer * GetResourceSource() const;
};
