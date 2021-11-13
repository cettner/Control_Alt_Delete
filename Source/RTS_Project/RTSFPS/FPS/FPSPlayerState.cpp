// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSPlayerState.h"

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
