// Fill out your copyright notice in the Description page of Project Settings.


#include "UpgradableInterface.h"

// Add default functionality here for any IUpg

uint32 IUpgradableInterface::GetCurrentUpgradeRankFor(const TSubclassOf<UUpgrade>& UpgradeClass) const
{
	return UPGRADE_UNLEARNED;
}


TArray<TSubclassOf<UUpgrade>> IUpgradableInterface::GetKnownUpgrades() const
{
	return TArray<TSubclassOf<UUpgrade>>();
}

TArray<TSubclassOf<UUpgrade>> IUpgradableInterface::GetUnknownUpgrades() const
{
	return TArray<TSubclassOf<UUpgrade>>();
}

TArray<TSubclassOf<UUpgrade>> IUpgradableInterface::GetAllUpgrades() const
{
	return TArray<TSubclassOf<UUpgrade>>();
}

bool IUpgradableInterface::CanSupportUpgrade(const TSubclassOf<UUpgrade>& UpgradeClass)
{
	return false;
}

bool IUpgradableInterface::CanReceiveUpgrades() const
{
	const bool retval = (GetUpgradeApplicationObject() != nullptr);
	return retval;
}

void IUpgradableInterface::OnApplyUpgrade(const UUpgrade * Upgrade)
{
	if (CanReceiveUpgrades() == true)
	{
		Upgrade->ApplyUpgrade(GetUpgradeApplicationObject());
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

UObject * IUpgradableInterface::GetUpgradeApplicationObject()
{
	UObject * retval = Cast<UObject>(this);
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
