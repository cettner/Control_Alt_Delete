// Fill out your copyright notice in the Description page of Project Settings.


#include "Upgrade.h"
#include "Interfaces/UpgradableInterface.h"

void UUpgrade::ApplyUpgrade(UObject * ToUpgrade) const
{

}

bool UUpgrade::CanUpgrade(const IUpgradableInterface * TestUpgrade) const
{
	if (TestUpgrade == nullptr) return false;

	bool retval = true;
	const TArray<TSubclassOf<UUpgrade>> exclusions = GetExclusiveConditions();
	const TArray<FUpgradeUnlockCondition> unlockconditions = GetUnlockConditions();
	const uint32 maxrank = GetMaxRank();
	
	if (TestUpgrade->GetCurrentUpgradeRankFor(GetClass()) >= maxrank)
	{
		retval = false;
		return retval;
	}

	for (int i = 0; i < unlockconditions.Num(); i++)
	{
		const TSubclassOf<UUpgrade> unlockparent = unlockconditions[i].GetParent();
		const uint32 unlockrank = unlockconditions[i].GetRank();

		const uint32 currentrank = TestUpgrade->GetCurrentUpgradeRankFor(unlockparent);

		if (currentrank < unlockrank)
		{
			retval = false;
			return retval;
		}

	}

	for (int i = 0; i < exclusions.Num(); i++)
	{
		const uint32 currentrank = TestUpgrade->GetCurrentUpgradeRankFor(exclusions[i]);
		if (currentrank > UPGRADE_UNLEARNED)
		{
			retval = false;
			return retval;
		}
	}

	return retval;
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
