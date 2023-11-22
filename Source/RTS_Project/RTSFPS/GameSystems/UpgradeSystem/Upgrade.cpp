// Fill out your copyright notice in the Description page of Project Settings.


#include "Upgrade.h"
#include "Interfaces/UpgradableInterface.h"

bool UUpgrade::ApplyUpgrade(UObject* ToUpgrade, const uint32 OldRank, const uint32 NewRank, const bool HasAuthority, const bool IsLocal) const
{
	return false;
}

uint32 UUpgrade::GetMaxRank() const
{
	return MaxRank;
}

FString UUpgrade::GetUpgradeDescription(uint32 InCurrentRank) const
{
	return TooltipInfo;
}

FName UUpgrade::GetUpgradeName() const
{
	return UpgradeName;
}

TArray<TSubclassOf<UUpgrade>> UUpgrade::GetExclusiveConditions() const
{
	return ExclusiveConditions;
}

TArray<FUpgradeUnlockCondition> UUpgrade::GetUnlockConditions() const
{
	return UnlockConditions;
}