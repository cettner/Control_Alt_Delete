// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSFPSUpgradeNodeWidget.h"
#include "RTS_Project/GameArchitecture/Game/RTFPSGameState.h"

void URTSFPSUpgradeNodeWidget::ApplyUpgrade(IUpgradableInterface* UpgradeUser) const
{
	int debug = 9;
}

bool URTSFPSUpgradeNodeWidget::CanPurchaseUpgrade() const
{
	bool retval = Super::CanPurchaseUpgrade();
	const IResourceGatherer * purchaser = GetResourceSource();

	if(retval == true && (purchaser != nullptr))
	{
		const FReplicationResourceMap prices = GetUpgradeCost();
		const FReplicationResourceMap carriedresource = purchaser->GetAllHeldResources();

		retval &= AResource::CanAfford(carriedresource, prices);
	}


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
	return GetOwningPlayerPawn<IResourceGatherer>();
}
