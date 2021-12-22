// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSFPSUpgradeNodeWidget.h"
#include "RTS_Project/GameArchitecture/Game/RTFPSGameState.h"

void URTSFPSUpgradeNodeWidget::ApplyUpgrade(IUpgradableInterface* UpgradeUser) const
{
	//GetOwningPlayer<AFPS
}

bool URTSFPSUpgradeNodeWidget::CanPurchaseUpgrade() const
{
	bool retval = Super::CanPurchaseUpgrade();
	
	if(retval == true)
	{
	}
	//GetOwningPlayerPawn<IReso>();


	return retval;
}

FReplicationResourceMap URTSFPSUpgradeNodeWidget::GetUpgradeCost() const
{
	FReplicationResourceMap retval = FReplicationResourceMap();
	const UWorld * world = GetWorld();
	const ARTFPSGameState * gs = world->GetGameState<ARTFPSGameState>();
	retval = gs->GetUnitPrice(UpgradeToApply);

	return retval;
}

IResourceGatherer * URTSFPSUpgradeNodeWidget::GetResourceSource() const
{
	return nullptr;
}
