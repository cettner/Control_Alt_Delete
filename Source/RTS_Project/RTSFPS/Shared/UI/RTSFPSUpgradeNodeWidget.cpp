// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSFPSUpgradeNodeWidget.h"
#include "RTS_Project/RTSFPS/GameArchitecture/RTFPSGameState.h"
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

IExpAccumulatorInterface * URTSFPSUpgradeNodeWidget::GetExpSource() const
{
	const APlayerController * pc = GetOwningPlayer();

	return(pc->GetPlayerState<IExpAccumulatorInterface>());
}
