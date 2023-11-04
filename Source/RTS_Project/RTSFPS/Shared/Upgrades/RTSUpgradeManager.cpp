// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSUpgradeManager.h"
#include "Net/UnrealNetwork.h"


void ARTSUpgradeManager::SetTeamID(int32 InTeamID)
{
	if (HasAuthority())
	{
		TeamID = InTeamID;
	}
}

void ARTSUpgradeManager::OnRep_TeamID()
{
}

void ARTSUpgradeManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ARTSUpgradeManager, Upgrades);
	DOREPLIFETIME_CONDITION(ARTSUpgradeManager, TeamID, COND_InitialOnly);
}
