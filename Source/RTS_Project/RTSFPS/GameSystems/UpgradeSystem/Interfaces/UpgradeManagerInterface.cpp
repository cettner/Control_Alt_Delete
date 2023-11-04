// Fill out your copyright notice in the Description page of Project Settings.


#include "UpgradeManagerInterface.h"

// Add default functionality here for any IUpgradeManagerInterface functions that are not pure virtual.

bool IUpgradeManagerInterface::HasAuthority()
{
	return false;
}

void IUpgradeManagerInterface::AddUpgrade(const TSubclassOf<UUpgrade>& InUpgrade)
{
}

void IUpgradeManagerInterface::RemoveUpgrade(const TSubclassOf<UUpgrade>& InUpgrade)
{
}

bool IUpgradeManagerInterface::RegisterUpgradable(IUpgradableInterface* InUpgradeObject)
{
	return false;
}

bool IUpgradeManagerInterface::UnRegisterUpgradable(IUpgradableInterface* InUpgradeObject)
{
	return false;
}

const TArray<const TSubclassOf<UUpgrade>> IUpgradeManagerInterface::GetAllUpgradesFor(IUpgradableInterface* InUpgradeObject) const
{
	return TArray<const TSubclassOf<UUpgrade>>();
}

const TArray<TScriptInterface<IUpgradableInterface>> IUpgradeManagerInterface::GetUpgradeSubScribers() const
{
	return TArray<TScriptInterface<IUpgradableInterface>>();
}

const TArray<const TSubclassOf<UUpgrade>> IUpgradeManagerInterface::GetAllAppliedUpgrades() const
{
	return TArray<const TSubclassOf<UUpgrade>>();
}
