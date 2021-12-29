// Fill out your copyright notice in the Description page of Project Settings.


#include "UpgradableInterface.h"

// Add default functionality here for any IUpg

uint32 IUpgradableInterface::GetCurrentUpgradeRankFor(TSubclassOf<UUpgrade> UpgradeClass) const
{
	return UPGRADE_UNLEARNED;
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
		AddUpgrade(Upgrade->GetClass());
	}
	else
	{
		AddUpgrade(Upgrade->GetClass());
	}
}

void IUpgradableInterface::PostInstallUpgrades()
{
}

const UObject * IUpgradableInterface::GetUpgradeApplicationObject() const
{
	const UObject * retval = Cast<UObject>(this);
	return retval;
}

UClass * IUpgradableInterface::GetUpgradeApplicationClass() const
{
	UClass * retval = nullptr;
	const UObject * subobject = GetUpgradeApplicationObject();
	if (subobject != nullptr)
	{
		retval = subobject->GetClass();
	}
	
	return retval;
}



bool IUpgradableInterface::AddUpgrade(TSubclassOf<UUpgrade> UpgradeToAdd)
{
	return false;
}
