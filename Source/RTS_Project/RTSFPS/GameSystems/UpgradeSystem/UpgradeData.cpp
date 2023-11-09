// Fill out your copyright notice in the Description page of Project Settings.


#include "UpgradeData.h"
#include "Net/UnrealNetwork.h"

TArray<TSubclassOf<UUpgrade>> UUpgradeData::GetAllUpgrades() const
{
	TArray<TSubclassOf<UUpgrade>> retval = TArray<TSubclassOf<UUpgrade>>();
	for (int32 i = 0; i < UpgradeState.Num(); i++)
	{
		retval.Emplace(UpgradeState[i].UpgradeClass);
	}

	return retval;
}

uint32 UUpgradeData::GetCurrentUpgradeRankFor(const TSubclassOf<UUpgrade>& UpgradeClass) const
{
	uint32 retval = UPGRADE_UNLEARNED;
	for (int32 i = 0; i < UpgradeState.Num(); i++)
	{
		if (UpgradeState[i].UpgradeClass == UpgradeClass)
		{
			retval = UpgradeState[i].Rank;
			break;
		}
	}
	return retval;
}

bool UUpgradeData::IncrementUpgrade(const TSubclassOf<UUpgrade>& InUpgradeToIncrement)
{
	bool retval = false;
	for (int32 i = 0; i < UpgradeState.Num(); i++)
	{
		if (InUpgradeToIncrement == UpgradeState[i])
		{
			const uint32 oldrank = UpgradeState[i].Rank;
			UpgradeState[i].Rank += 1U;
			
			OnUpgradeChanged.Broadcast(InUpgradeToIncrement, oldrank, UpgradeState[i].Rank);
			retval = true;
			break;
		}
	}
	return retval;
}

bool UUpgradeData::DecrementUpgrade(const TSubclassOf<UUpgrade>& UpgradeToDecrement)
{
	bool retval = false;
	for (int32 i = 0; i < UpgradeState.Num(); i++)
	{
		if (UpgradeToDecrement == UpgradeState[i])
		{
			const uint32 oldrank = UpgradeState[i].Rank;
			UpgradeState[i].Rank -= 1U;

			OnUpgradeChanged.Broadcast(UpgradeToDecrement, oldrank, UpgradeState[i].Rank);
			retval = true;
			break;
		}
	}
	return retval;
}

void UUpgradeData::OnRep_UpgradeState(TArray<FUpgradeInfo> OldUpgrades)
{
	for (int32 i = 0; i < UpgradeState.Num(); i++)
	{
		if (!OldUpgrades.Contains(UpgradeState[i]))
		{
			TSubclassOf<UUpgrade> upgradclass = UpgradeState[i].UpgradeClass;
			if (i < OldUpgrades.Num())
			{
				OnUpgradeChanged.Broadcast(upgradclass, OldUpgrades[i].Rank, UpgradeState[i].Rank);
			}
			else
			{
				OnUpgradeChanged.Broadcast(upgradclass, UPGRADE_UNLEARNED, UpgradeState[i].Rank);
			}
		}
	}
}

void UUpgradeData::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UUpgradeData, UpgradeState);
}

bool UUpgradeData::IsSupportedForNetworking() const
{
	return true;
}
