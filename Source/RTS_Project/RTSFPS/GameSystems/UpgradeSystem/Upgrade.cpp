// Fill out your copyright notice in the Description page of Project Settings.


#include "Upgrade.h"
#include "Interfaces/UpgradableInterface.h"

void UUpgrade::ApplyUpgrade(IUpgradableInterface * ToUpgrade) const
{

}

bool UUpgrade::CanUpgrade(IUpgradableInterface * TestUpgrade) const
{
	return false;
}

uint32 UUpgrade::GetMaxRank() const
{
	return MaxRank;
}

FText UUpgrade::GetToolTipInfo(uint32 CurrentRank) const
{
	return TooltipInfo;
}

TArray<TSubclassOf<UUpgrade>> UUpgrade::GetExclusiveConditions() const
{
	return ExclusiveConditions;
}

TArray<FUpgradeUnlockCondition> UUpgrade::GetUnlockConditions() const
{
	return UnlockConditions;
}
