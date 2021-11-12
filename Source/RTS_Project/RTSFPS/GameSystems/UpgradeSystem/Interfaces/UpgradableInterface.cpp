// Fill out your copyright notice in the Description page of Project Settings.


#include "UpgradableInterface.h"

// Add default functionality here for any IUpg

int IUpgradableInterface::GetCurrentUpgradeTierFor(TSubclassOf<UUpgrade> UpgradeClass)
{
	return -1;
}


int IUpgradableInterface::GetMaxUpgradeTierFor(TSubclassOf<UUpgrade> UpgradeClass)
{
	return 0;
}

TArray<TSubclassOf<UUpgrade>> IUpgradableInterface::GetAppliedUpgrades()
{
	return TArray<TSubclassOf<UUpgrade>>();
}

bool IUpgradableInterface::IsPreInitialized() const
{
	return false;
}

void IUpgradableInterface::OnApplyUpgrade(const UUpgrade * Upgrade)
{
		Upgrade->ApplyUpgrade(this);
}

void IUpgradableInterface::PostInstallUpgrades()
{
}

bool IUpgradableInterface::AddUpgrade(TSubclassOf<UUpgrade> UpgradeToAdd)
{
	return false;
}
