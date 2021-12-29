// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSUpgrade.h"
#include "RTS_Project/RTSFPS/GameSystems/UpgradeSystem/Interfaces/UpgradableInterface.h"

bool URTSUpgrade::CanUpgrade(const IUpgradableInterface * TestUpgrade) const
{
	bool retval = UUpgrade::CanUpgrade(TestUpgrade);
	const UClass * actorclass = TestUpgrade->GetUpgradeApplicationClass();
	bool hasclass = false;

	for (int i = 0; i < TargetClasses.Num(); i++)
	{
		hasclass |= actorclass->IsChildOf(TargetClasses[i].Get());
	}

	retval &= hasclass;
	return(retval);
}

bool URTSUpgrade::IsGlobal() const
{
	return bIsGlobal;
}

bool URTSUpgrade::IsPersistent() const
{
	return bPersistsThroughRespawn;
}
