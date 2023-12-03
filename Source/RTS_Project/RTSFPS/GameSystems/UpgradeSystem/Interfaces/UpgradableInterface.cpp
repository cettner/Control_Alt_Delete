// Fill out your copyright notice in the Description page of Project Settings.


#include "UpgradableInterface.h"

// Add default functionality here for any IUpg

uint32 IUpgradableInterface::GetCurrentUpgradeRankFor(const TSubclassOf<UUpgrade>& UpgradeClass) const
{
	return UPGRADE_UNLEARNED;
}

TArray<TSubclassOf<UUpgrade>> IUpgradableInterface::GetAllUpgrades() const
{
	return TArray<TSubclassOf<UUpgrade>>();
}

TArray<TSubclassOf<UUpgrade>> IUpgradableInterface::GetInstalledUpgrades() const
{
	return TArray<TSubclassOf<UUpgrade>>();
}

/*********************************************************************************************************/
bool IUpgradableInterface::MeetsUpgradeDependencies(const TSubclassOf<UUpgrade>& UpgradeClass) const
{
	const UUpgrade* upgradecdo = UpgradeClass.GetDefaultObject();
	bool retval = true;
	const uint32 maxrank = upgradecdo->GetMaxRank();

	if (GetCurrentUpgradeRankFor(UpgradeClass) >= maxrank)
	{
		retval = false;
		return retval;
	}

	const TArray<TSubclassOf<UUpgrade>> exclusions = upgradecdo->GetExclusiveConditions();
	const TArray<FUpgradeUnlockCondition> unlockconditions = upgradecdo->GetUnlockConditions();

	for (int i = 0; i < unlockconditions.Num(); i++)
	{
		const TSubclassOf<UUpgrade> unlockparent = unlockconditions[i].GetParent();
		const uint32 unlockrank = unlockconditions[i].GetRank();

		const uint32 currentrank = GetCurrentUpgradeRankFor(unlockparent);

		if (currentrank < unlockrank)
		{
			retval = false;
			return retval;
		}

	}

	for (int i = 0; i < exclusions.Num(); i++)
	{
		const uint32 currentrank = GetCurrentUpgradeRankFor(exclusions[i]);
		if (currentrank > UPGRADE_UNLEARNED)
		{
			retval = false;
			return retval;
		}
	}

	return retval;
}

bool IUpgradableInterface::MeetsUpgradeDependencies(const TSubclassOf<UUpgrade>& UpgradeClass, TArray<FUpgradeDependencyInfo>& OutDependencyInfo) const
{
	const UUpgrade* upgradecdo = UpgradeClass.GetDefaultObject();
	bool retval = true;

	const TArray<TSubclassOf<UUpgrade>> exclusions = upgradecdo->GetExclusiveConditions();
	const TArray<FUpgradeUnlockCondition> unlockconditions = upgradecdo->GetUnlockConditions();
	const uint32 maxrank = upgradecdo->GetMaxRank();

	if (GetCurrentUpgradeRankFor(UpgradeClass) >= maxrank)
	{
		retval &= false;
	}

	for (int i = 0; i < unlockconditions.Num(); i++)
	{
		const TSubclassOf<UUpgrade> unlockparent = unlockconditions[i].GetParent();
		const uint32 unlockrank = unlockconditions[i].GetRank();
		const uint32 currentrank = GetCurrentUpgradeRankFor(unlockparent);

		FUpgradeDependencyInfo parentdependency;
		parentdependency.Description = "Requires " + FString::FromInt(unlockrank) + " Points in " + unlockparent.GetDefaultObject()->GetUpgradeName().ToString();
		parentdependency.Category = EUpgradeDependencyCategory::UNLOCK;

		if (currentrank < unlockrank)
		{
			retval &= false;
			parentdependency.IsSatisfied = false;
		}
		else
		{
			parentdependency.IsSatisfied = true;
		}
		OutDependencyInfo.Emplace(parentdependency);
	}



	for (int i = 0; i < exclusions.Num(); i++)
	{
		const UUpgrade* exclusionupgrade = exclusions[i].GetDefaultObject();

		FUpgradeDependencyInfo parentdependency;
		parentdependency.Description = "Exclusive With : " + exclusionupgrade->GetUpgradeName().ToString();
		parentdependency.Category = EUpgradeDependencyCategory::EXCLUSION;

		const uint32 currentrank = GetCurrentUpgradeRankFor(exclusions[i]);
		if (currentrank > UPGRADE_UNLEARNED)
		{
			retval &= false;
			parentdependency.IsSatisfied = false;
		}
		else
		{
			parentdependency.IsSatisfied = true;
		}
		OutDependencyInfo.Emplace(parentdependency);
	}


	return retval;
}

bool IUpgradableInterface::MeetsRemovalDependencies(const TSubclassOf<UUpgrade>& InUpgradeClass) const
{
	const uint32 currentrank = GetCurrentUpgradeRankFor(InUpgradeClass);
	if (currentrank == UPGRADE_UNLEARNED) return false;

	bool retval = true;
	const uint32 removedrank = currentrank - 1U;
	const TArray<TSubclassOf<UUpgrade>> knownupgrades = GetKnownUpgrades();

	for (int i = 0; i < knownupgrades.Num(); i++)
	{
		const UUpgrade* upgradecdo = knownupgrades[i].GetDefaultObject();
		const TArray<FUpgradeUnlockCondition> unlockconditions = upgradecdo->GetUnlockConditions();

		for (int y = 0; y < unlockconditions.Num(); y++)
		{
			const FUpgradeUnlockCondition unlockcondition = unlockconditions[y];

			if ((unlockcondition.GetParent() == InUpgradeClass) && (unlockcondition.GetRank() > removedrank))
			{
				return false;
			}
		}
	}
	return true;
}

bool IUpgradableInterface::MeetsRemovalDependencies(const TSubclassOf<UUpgrade>& InUpgradeClass, TArray<TSubclassOf<UUpgrade>>& OutViolatedUpgrades) const
{
	OutViolatedUpgrades.Reset();
	const uint32 currentrank = GetCurrentUpgradeRankFor(InUpgradeClass);
	if (currentrank == UPGRADE_UNLEARNED) return false;

	bool retval = true;
	const uint32 removedrank = currentrank - 1U;
	const TArray<TSubclassOf<UUpgrade>> knownupgrades = GetKnownUpgrades();
	
	for (int i = 0; i < knownupgrades.Num(); i++)
	{
		UUpgrade* upgradecdo = knownupgrades[i].GetDefaultObject();
		const TArray<FUpgradeUnlockCondition> unlockconditions = upgradecdo->GetUnlockConditions();
		
		for (int y = 0; y < unlockconditions.Num(); y++)
		{
			const FUpgradeUnlockCondition unlockcondition = unlockconditions[y];

			if ((unlockcondition.GetParent() == InUpgradeClass) && (unlockcondition.GetRank() < removedrank))
			{
				OutViolatedUpgrades.Emplace(unlockcondition.GetParent());
				retval = false;
			}
		}
	}
	
	return retval;
}

TArray<TSubclassOf<UUpgrade>> IUpgradableInterface::GetKnownUpgrades() const
{
	TArray<TSubclassOf<UUpgrade>> retval = TArray<TSubclassOf<UUpgrade>>();
	const TArray<TSubclassOf<UUpgrade>> allupgrades = GetAllUpgrades();
	for (TSubclassOf<UUpgrade> upgrade : allupgrades)
	{
		if (GetCurrentUpgradeRankFor(upgrade) > UPGRADE_UNLEARNED)
		{
			retval.Emplace(upgrade);
		}
	}
	return retval;
}

TArray<TSubclassOf<UUpgrade>> IUpgradableInterface::GetUnknownUpgrades() const
{
	TArray<TSubclassOf<UUpgrade>> retval = TArray<TSubclassOf<UUpgrade>>();
	const TArray<TSubclassOf<UUpgrade>> allupgrades = GetAllUpgrades();
	for (TSubclassOf<UUpgrade> upgrade : allupgrades)
	{
		if (GetCurrentUpgradeRankFor(upgrade) == UPGRADE_UNLEARNED)
		{
			retval.Emplace(upgrade);
		}
	}
	return retval;
}

bool IUpgradableInterface::InstallUpgrade(const TSubclassOf<UUpgrade>& InUpgrade, const uint32 InRank)
{
	bool retval = CanSupportUpgrade(InUpgrade);
	checkf(retval, TEXT("IUpgradableInterface::InstallUpgrade Upgrade was not supported"));
	
	const uint32 currentrank = GetCurrentUpgradeRankFor(InUpgrade);

	if (InRank > currentrank)
	{
		for (uint32 i = currentrank; i < InRank; i++)
		{
			if (i == UPGRADE_UNLEARNED)
			{
				retval &= LearnUpgrade(InUpgrade);
			}
			else
			{
				retval &= IncrementUpgrade(InUpgrade);
			}
		}
	}
	else if (InRank < currentrank)
	{
		for (uint32 i = currentrank; i > InRank; i--)
		{
			if (i == 1U)
			{
				retval &= UnLearnUpgrade(InUpgrade);
			}
			else
			{
				retval &= DecrementUpgrade(InUpgrade);
			}
		}
	}

	return retval;
}

bool IUpgradableInterface::InstallUpgrade(const TSubclassOf<UUpgrade>& InUpgrade)
{
	bool retval = false;
	const UUpgrade* upgradecdo = InUpgrade.GetDefaultObject();
	const uint32 currentrank = GetCurrentUpgradeRankFor(InUpgrade);
	const uint32 maxrank = upgradecdo->GetMaxRank();

	if (currentrank < maxrank)
	{
		const uint32 newrank = currentrank + 1U;
		retval = InstallUpgrade(InUpgrade, newrank);
	}

	return retval;
}

bool IUpgradableInterface::UninstallUpgrade(const TSubclassOf<UUpgrade>& InUpgrade)
{
	const uint32 currentrank = GetCurrentUpgradeRankFor(InUpgrade);
	if (currentrank == UPGRADE_UNLEARNED) return false;
	
	const uint32 newrank = currentrank - 1U;

	return InstallUpgrade(InUpgrade, newrank);
}

bool IUpgradableInterface::MaxInstallUpgrade(const TSubclassOf<UUpgrade>& InUpgrade)
{
	const UUpgrade* upgradecdo = InUpgrade.GetDefaultObject();
	const uint32 maxrank = upgradecdo->GetMaxRank();
	
	bool retval = InstallUpgrade(InUpgrade, maxrank);

	return retval;
}

bool IUpgradableInterface::CanSupportUpgrade(const TSubclassOf<UUpgrade>& UpgradeClass) const
{
	return GetAllUpgrades().Contains(UpgradeClass);
}

bool IUpgradableInterface::LearnUpgrade(const TSubclassOf<UUpgrade>& UpgradeToAdd)
{
	return IncrementUpgrade(UpgradeToAdd);
}

bool IUpgradableInterface::UnLearnUpgrade(const TSubclassOf<UUpgrade>& UpgradeToUnLearn)
{
	return DecrementUpgrade(UpgradeToUnLearn);
}
