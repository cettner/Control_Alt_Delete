// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSUpgradeManager.h"
#include "../../GameArchitecture/RTFPSGameState.h"

#include "Net/UnrealNetwork.h"


void ARTSUpgradeManager::SetTeamID(int32 InTeamID)
{
	if (HasAuthority())
	{
		TeamID = InTeamID;
		OnRep_TeamID();
	}
}

void ARTSUpgradeManager::OnRep_TeamID()
{
	if (HasAuthority())
	{

	}
}

void ARTSUpgradeManager::RegisterMinion(ARTSMinion* InMinion)
{
	const TSubclassOf<ARTSMinion> minionclass = InMinion->GetClass();

	if (HasAuthority())
	{
		if (const int32* foundindex = IndexMapping.Find(minionclass))
		{
			UUpgradeData* currentupgradeset = Upgrades[*foundindex];
			for (const FUpgradeInfo& upgrade : currentupgradeset->GetUpgradeState())
			{
//				InMinion->InstallUpgrade(upgrade.UpgradeClass, upgrade.Rank);
			}

			currentupgradeset->OnUpgradeChanged.AddUFunction(InMinion, "OnUpgradeChanged");
		}
		else
		{
			const TSubclassOf<UUpgradeData> upgradesetclass = minionclass.GetDefaultObject()->GetUpgradeDataClass();
			UUpgradeData* upgradeset = NewObject<UUpgradeData>(this, upgradesetclass);
			int32 upgradeindex = Upgrades.Emplace(upgradeset);
			IndexMapping.Emplace(minionclass, upgradeindex);
		}
	}
}

void ARTSUpgradeManager::UnRegisterMinion(ARTSMinion* InMinion)
{
}

void ARTSUpgradeManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ARTSUpgradeManager, Upgrades);
	DOREPLIFETIME_CONDITION(ARTSUpgradeManager, TeamID, COND_InitialOnly);
}
