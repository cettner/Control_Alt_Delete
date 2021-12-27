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
	return CurrentExperiance;
}

uint32 AFPSPlayerState::GetMaxExpForCurrentLevel() const
{
	const uint32 retval = GetExpforLevel(GetCurrentLevel());
	return retval;
}

bool AFPSPlayerState::CanRecieveExp() const
{
	const bool retval = GetCurrentLevel() < GetMaxLevel();
	return retval;
}

uint32 AFPSPlayerState::GetCurrentLevel() const
{
	return TotalUpgradePoints;
}

uint32 AFPSPlayerState::GetMaxLevel() const
{
	return uint32();
}

uint32 AFPSPlayerState::GetExptoNextLevel() const
{
	return uint32();
}

uint32 AFPSPlayerState::GetExpforLevel(uint32 inLevel) const
{
	checkf(ExpCurve, TEXT("AFPSPlayerState::GetExpforLevel ExpCurve was Null"))

	const uint32 outval = (uint32)ExpCurve->GetFloatValue(inLevel);
	
	return outval;
}

void AFPSPlayerState::GrantExp(uint32 inexp)
{

	CurrentExperiance += inexp;

}

void AFPSPlayerState::OnLevelUp()
{
	TotalUpgradePoints += 1U;

	
}

void AFPSPlayerState::LoadGameModeDefaults(const AGameModeBase* GameModeCDO)
{
	Super::LoadGameModeDefaults(GameModeCDO);
}

void AFPSPlayerState::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (HasAuthority())
	{
		const UWorld * world = GetWorld();
		const ARTFPSMode * gm = world->GetAuthGameMode<ARTFPSMode>();
		checkf(gm, TEXT("AFPSPlayerState::PostInitializeComponents Server Failed to Obtain GameMode"))

		MaxLevel= gm->GetMaxLevel();
		ExpCurve = gm->GetExpCurve();
	}
}

void AFPSPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(AFPSPlayerState, TotalUpgradePoints, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AFPSPlayerState, SpentUpgradePoints, COND_OwnerOnly);
}
