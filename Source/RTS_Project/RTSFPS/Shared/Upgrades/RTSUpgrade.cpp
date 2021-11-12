// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSUpgrade.h"
#include "RTS_Project/RTSFPS/GameSystems/UpgradeSystem/Interfaces/UpgradableInterface.h"

bool URTSUpgrade::CanUpgrade(IUpgradableInterface * TestUpgrade) const
{
	bool retval = false;
	UObject * actorclass = Cast<UObject>(TestUpgrade);
	for (int i = 0; i < TargetClasses.Num(); i++)
	{
		retval |= actorclass->GetClass()->IsChildOf(TargetClasses[i].Get());
	}

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
