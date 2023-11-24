// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSPlayerState.h"
#include "RTS_Project/RTSFPS/GameArchitecture/RTFPSMode.h"
#include "RTS_Project/RTSFPS/GameArchitecture/RTFPSGameState.h"
#include "GameFramework/GameStateBase.h"
#include "Net/UnrealNetwork.h"

AFPSPlayerState::AFPSPlayerState() : Super()
{
	bReplicateUsingRegisteredSubObjectList = true;
}

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

}

TArray<TSubclassOf<UUpgrade>> AFPSPlayerState::GetAllUpgrades() const
{
	TArray<TSubclassOf<UUpgrade>> retval = TArray<TSubclassOf<UUpgrade>>();
	if (IsValid(UpgradeData))
	{
		retval = UpgradeData->GetAllUpgrades();
	}
	return retval;
}

uint32 AFPSPlayerState::GetCurrentUpgradeRankFor(const TSubclassOf<UUpgrade>& UpgradeClass) const
{
	checkf(UpgradeClass, TEXT("AFPSPlayerState::GetCurrentUpgradeRankFor : UpgradeClass was Null"))
	uint32 retval = UPGRADE_UNLEARNED;
	if (IsValid(UpgradeData))
	{
		retval = UpgradeData->GetCurrentUpgradeRankFor(UpgradeClass);
	}

	return retval;
}

bool AFPSPlayerState::IncrementUpgrade(const TSubclassOf<UUpgrade>& UpgradeToIncrement)
{
	return UpgradeData->IncrementUpgrade(UpgradeToIncrement);
}

bool AFPSPlayerState::DecrementUpgrade(const TSubclassOf<UUpgrade>& UpgradeToDecrement)
{
	return UpgradeData->DecrementUpgrade(UpgradeToDecrement);
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
	const int32 retval = static_cast<int32>(GetMaxExpForCurrentLevel()) - static_cast<int32>(GetCurrentExp());
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

void AFPSPlayerState::RefundUpgradePoints(const uint32 InPointsToRefund)
{
	const int32 newspenttotal = static_cast<int32>(GetSpentUpgradePoints()) -  static_cast<int32>(InPointsToRefund);
	if (newspenttotal >= 0)
	{
		SetSpentUpgradePoints(newspenttotal);
	}
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
		
		const TSubclassOf<ACommander> commanderclass = gm->GetDefaultFPSClass();
		const ACommander* commandercdo = commanderclass.GetDefaultObject();
		const TSubclassOf<UUpgradeData> upgradedataclass = commandercdo->GetUpgradeDataClass();

		UpgradeData = NewObject<UUpgradeData>(this, upgradedataclass);
		AddReplicatedSubObject(UpgradeData);
	}
}

void AFPSPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(AFPSPlayerState, TotalUpgradePoints, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AFPSPlayerState, SpentUpgradePoints, COND_OwnerOnly);
	DOREPLIFETIME_CONDITION(AFPSPlayerState, CurrentExperiance, COND_OwnerOnly);
	/*Other players need to know the local upgrade state to view local effects*/
	DOREPLIFETIME(AFPSPlayerState, UpgradeData);

}
