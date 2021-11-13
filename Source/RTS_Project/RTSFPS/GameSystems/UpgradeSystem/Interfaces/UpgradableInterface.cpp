// Fill out your copyright notice in the Description page of Project Settings.


#include "UpgradableInterface.h"

// Add default functionality here for any IUpg

int IUpgradableInterface::GetCurrentUpgradeTierFor(TSubclassOf<UUpgrade> UpgradeClass) const
{
	return -1;
}


int IUpgradableInterface::GetMaxUpgradeTierFor(TSubclassOf<UUpgrade> UpgradeClass) const
{
	return 0;
}

TArray<TSubclassOf<UUpgrade>> IUpgradableInterface::GetAppliedUpgrades() const
{
	return TArray<TSubclassOf<UUpgrade>>();
}

bool IUpgradableInterface::CanReceiveUpgrades() const
{
	return true;
}

void IUpgradableInterface::OnApplyUpgrade(const UUpgrade * Upgrade)
{
	if (CanReceiveUpgrades() == true)
	{
		Upgrade->ApplyUpgrade(this);
	}
	else
	{
		AddUpgrade(Upgrade->GetClass());
	}
}

void IUpgradableInterface::PostInstallUpgrades()
{
}

bool IUpgradableInterface::AddUpgrade(TSubclassOf<UUpgrade> UpgradeToAdd)
{
	return false;
}
