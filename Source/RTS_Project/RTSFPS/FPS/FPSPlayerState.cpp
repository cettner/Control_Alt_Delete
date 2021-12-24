// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSPlayerState.h"
#include "RTS_Project/GameArchitecture/Game/RTFPSMode.h"

#include "GameFramework/GameStateBase.h"
#include "Net/UnrealNetwork.h"


EPlayerReswpawnState AFPSPlayerState::GetRespawnState() const
{
	return RespawnState;
}

void AFPSPlayerState::SetRespawnState(EPlayerReswpawnState NewState)
{
	RespawnState = NewState;
}

bool AFPSPlayerState::AddUpgrade(TSubclassOf<UUpgrade> UpgradeToAdd)
{
	FUpgradeInfo upgradewrapper;
	upgradewrapper.UpgradeClass = UpgradeToAdd;
	upgradewrapper.Rank = 1;

	int findindex = AppliedUpgrades.Find(upgradewrapper);
	if (findindex != INDEX_NONE)
	{
		AppliedUpgrades[findindex].Rank++;
	}
	else
	{
		AppliedUpgrades.Emplace(upgradewrapper);
	}

	return true;
}

TArray<TSubclassOf<UUpgrade>> AFPSPlayerState::GetAppliedUpgrades() const
{
	TArray<TSubclassOf<UUpgrade>> retval = TArray<TSubclassOf<UUpgrade>>();
	for (int i = 0; i < AppliedUpgrades.Num(); i++)
	{
		retval.Emplace(AppliedUpgrades[i].UpgradeClass);
	}

	return retval;
}

uint32 AFPSPlayerState::GetCurrentExp() const
{
	return uint32();
}

uint32 AFPSPlayerState::GetMaxExpForLevel() const
{
	return uint32();
}

bool AFPSPlayerState::CanRecieveExp() const
{
	return false;
}

uint32 AFPSPlayerState::GetCurrentLevel() const
{
	return uint32();
}

uint32 AFPSPlayerState::GetMaxLevel() const
{
	return uint32();
}

uint32 AFPSPlayerState::GetExptoNextLevel() const
{
	return uint32();
}

void AFPSPlayerState::GrantExp(uint32 inexp)
{
}

void AFPSPlayerState::OnLevelUp()
{
}

void AFPSPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AFPSPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(AFPSPlayerState, TotalUpgradePoints, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AFPSPlayerState, SpentUpgradePoints, COND_OwnerOnly);
}
