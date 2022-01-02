// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSFPSUpgradeNodeWidget.h"
#include "RTS_Project/GameArchitecture/Game/RTFPSGameState.h"
#include "RTS_Project/RTSFPS/FPS/FPSServerController.h"

void URTSFPSUpgradeNodeWidget::ApplyUpgrade(IUpgradableInterface* UpgradeUser) const
{
	AFPSServerController * pc = GetOwningPlayer<AFPSServerController>();
	pc->PurchaseExpUpgrade(UpgradeToApply);
}

bool URTSFPSUpgradeNodeWidget::CanPurchaseUpgrade() const
{
	bool retval = Super::CanPurchaseUpgrade();
	const IExpAccumulatorInterface * purchaser = GetExpSource();

	if(retval == true && (purchaser != nullptr))
	{
		retval &= (purchaser->GetAvailableUpgradePoints() > 0U);
	}


	return retval;
}

FReplicationResourceMap URTSFPSUpgradeNodeWidget::GetUpgradeResourceCost() const
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

IExpAccumulatorInterface * URTSFPSUpgradeNodeWidget::GetExpSource() const
{
	const APlayerController * pc = GetOwningPlayer();

	return(pc->GetPlayerState<IExpAccumulatorInterface>());
}
