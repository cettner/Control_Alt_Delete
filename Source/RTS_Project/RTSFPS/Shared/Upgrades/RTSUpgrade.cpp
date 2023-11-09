// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSUpgrade.h"
#include "RTS_Project/RTSFPS/GameSystems/UpgradeSystem/Interfaces/UpgradableInterface.h"

bool URTSUpgrade::IsGlobal() const
{
	return bIsGlobal;
}

bool URTSUpgrade::IsPersistent() const
{
	return bPersistsThroughRespawn;
}

void URTSUpgrade::RegisterRTSObject()
{
}

void URTSUpgrade::UnRegisterRTSObject()
{
}
