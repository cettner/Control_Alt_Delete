// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSPlayerState.h"
#include "RTS_Project/RTSFPS/GameArchitecture/RTFPSMode.h"
#include "RTS_Project/RTSFPS/GameArchitecture/RTFPSGameState.h"
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

void AFPSPlayerState::SetTotalUpgradePoints(uint32 InTotalUpgradePoints)
{
	TotalUpgradePoints = InTotalUpgradePoints;
	if (HasAuthority() && IsLocalPlayerState())
	{
		OnRep_TotalUpgradePoints();
	}
}

void AFPSPlayerState::SetSpentUpgradePoints(uint32 InSpentUpgradePoints)
{
	SpentUpgradePoints = InSpentUpgradePoints;
	if (HasAuthority() && IsLocalPlayerState())
	{
		OnRep_SpentUpgradePoints();
	}
}

void AFPSPlayerState::OnRep_TotalUpgradePoints()
{
	const UWorld * world = GetWorld();
	ARTSFPSHUD * hud = world->GetFirstPlayerController()->GetHUD<ARTSFPSHUD>();
	hud->RefreshUpgradeTree();
}

void AFPSPlayerState::OnRep_SpentUpgradePoints()
{
	const UWorld* world = GetWorld();
	ARTSFPSHUD* hud = world->GetFirstPlayerController()->GetHUD<ARTSFPSHUD>();
	hud->RefreshUpgradeTree();
}

void AFPSPlayerState::OnRep_AppliedUpgrades()
{
	const UWorld* world = GetWorld();
	ARTSFPSHUD* hud = world->GetFirstPlayerController()->GetHUD<ARTSFPSHUD>();
	hud->RefreshUpgradeTree();
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

	if (HasAuthority() && IsLocalPlayerState())
	{
		OnRep_AppliedUpgrades();
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

UClass * AFPSPlayerState::GetUpgradeApplicationClass() const
{
	UClass * retval = nullptr;
	const UWorld * world = GetWorld();

	if (HasAuthority() && !IsRTSPlayer())
	{
		const ARTFPSMode * gm = world->GetAuthGameMode<ARTFPSMode>();
		retval = gm->GetDefaultFPSClass();
	}
	else if(!IsRTSPlayer())
	{
		const ARTFPSGameState * gs = world->GetGameState<ARTFPSGameState>();
		const ARTFPSMode * gm = gs->GetDefaultGameMode<ARTFPSMode>();
		if (gm != nullptr)
		{
			retval = gm->GetDefaultFPSClass();
		}
	}

	return retval;
}

UObject * AFPSPlayerState::GetUpgradeApplicationObject()
{
	APawn * retval = GetPawn();
	const UClass * upgradeclass = GetUpgradeApplicationClass();
	bool isfpsclasspawn = IsValid(retval) && (retval->GetClass() == upgradeclass);
	
	if (isfpsclasspawn == false)
	{
		retval = nullptr;
	}

	return retval;
}

const UObject * AFPSPlayerState::GetUpgradeApplicationObject() const
{
	APawn * retval = GetPawn();
	const UClass * upgradeclass = GetUpgradeApplicationClass();
	bool isfpsclasspawn = IsValid(retval) && (retval->GetClass() == upgradeclass);

	if (isfpsclasspawn == false)
	{
		retval = nullptr;
	}

	return retval;
}

uint32 AFPSPlayerState::GetCurrentUpgradeRankFor(TSubclassOf<UUpgrade> UpgradeClass) const
{
	checkf(UpgradeClass, TEXT("AFPSPlayerState::GetCurrentUpgradeRankFor : UpgradeClass was Null"))
	uint32 retval = UPGRADE_UNLEARNED;
	for (int i = 0; i < AppliedUpgrades.Num(); i++)
	{
		if (UpgradeClass == AppliedUpgrades[i].UpgradeClass)
		{
			retval = AppliedUpgrades[i].Rank;
			break;
		}
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
	return MaxLevel;
}

int32 AFPSPlayerState::GetExptoNextLevel() const
{
	/*WARNING: This value can be negative, so we do lose a bit in this conversion*/
	const int32 retval = (int32)GetMaxExpForCurrentLevel() - (int32)GetCurrentExp();
	return retval;
}

uint32 AFPSPlayerState::GetExpforLevel(uint32 inLevel) const
{
	checkf(ExpCurve, TEXT("AFPSPlayerState::GetExpforLevel : ExpCurve was Null"))

	const uint32 outval = (uint32)ExpCurve->GetFloatValue(inLevel);
	
	return outval;
}

uint32 AFPSPlayerState::GetAvailableUpgradePoints() const
{
	checkf(GetTotalUpgradePoints() >= GetSpentUpgradePoints(), TEXT(" AFPSPlayerState::GetAvailableUpgradePoints : Total UpgradePoints Was less than Spent"))
	return GetTotalUpgradePoints() - GetSpentUpgradePoints();
}

uint32 AFPSPlayerState::GetSpentUpgradePoints() const
{
	return SpentUpgradePoints;
}

uint32 AFPSPlayerState::GetTotalUpgradePoints() const
{
	return TotalUpgradePoints;
}

bool AFPSPlayerState::SpendUpgradePoints(uint32 PointsToSpend)
{
	bool retval = false;
	if (PointsToSpend <= GetAvailableUpgradePoints())
	{
		const uint32 newspentotal = GetSpentUpgradePoints() + PointsToSpend;
		SetSpentUpgradePoints(newspentotal);
		retval = true;
	}
	return retval;
}

void AFPSPlayerState::GrantExp(uint32 inexp)
{
	if (CanRecieveExp() == true)
	{
		CurrentExperiance += inexp;
		const int32 expttonextlevel = GetExptoNextLevel();
		if (expttonextlevel <= 0)
		{
			OnLevelUp();
		}
	}

}

void AFPSPlayerState::OnLevelUp()
{
	/*Increment the Level*/
	const uint32 newupgradepoints = GetTotalUpgradePoints() + 1U;
	SetTotalUpgradePoints(newupgradepoints);

	/*Max Level, just set Current Exp to 0*/
	if (GetCurrentLevel() == GetMaxLevel())
	{
		CurrentExperiance = 0;
	}
	else
	{
		/*Get the amount of overflow if any and apply it to the next level, this may trigger recursive calls*/
		const int32 remainder = GetExptoNextLevel() * -1;
		CurrentExperiance = 0;

		if (remainder > 0)
		{
			GrantExp(remainder);
		}

	}
	
}

void AFPSPlayerState::LoadGameModeDefaults(const AGameModeBase* GameModeCDO)
{
	Super::LoadGameModeDefaults(GameModeCDO);
	/*Load Local Data that is non-gameplay relevant*/
	if (!HasAuthority())
	{
		const ARTFPSMode * gm = Cast<ARTFPSMode>(GameModeCDO);

		MaxLevel = gm->GetMaxLevel();
		ExpCurve = gm->GetExpCurve();
	}
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
	DOREPLIFETIME_CONDITION(AFPSPlayerState, CurrentExperiance, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AFPSPlayerState, AppliedUpgrades, COND_OwnerOnly);

}
