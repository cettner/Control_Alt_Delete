// Fill out your copyright notice in the Description page of Project Settings.

#include "RTFPSGameState.h"
#include "RTFPSPlayerState.h"

ARTFPSGameState::ARTFPSGameState(const FObjectInitializer &FOI) : Super(FOI)
{
	AllUnits = TArray<RTSTeamUnits>();
}

int ARTFPSGameState::NumRTSPlayers(int Team_Index)
{
	int retval = 0;
	if(IsTeamValid(Team_Index))
	{
		for (int i = 0; i < Teams[Team_Index].Num(); i++)
		{
			ARTFPSPlayerState * PS = Cast<ARTFPSPlayerState>(Teams[Team_Index][i]);
			if (PS && PS->isRtsPlayer)
			{
				retval++;
			}
		}
	}
	return(retval);
}

void ARTFPSGameState::RefreshAllUnits()
{
	for (TObjectIterator<ARTSMinion> Itr; Itr; ++Itr)
	{
		ARTSMinion* minion = *Itr;
		if (minion && IsTeamValid(minion->GetTeam()))
		{
			AllUnits[minion->GetTeam()].Minions.AddUnique(minion);
		}
		else
		{
			InvalidUnits.Minions.Add(minion);
		}
	}

	for (TObjectIterator<ARTSStructure> Itr; Itr; ++Itr)
	{
		ARTSStructure* structure = *Itr;
		if (structure && IsTeamValid(structure->GetTeam()))
		{
			AllUnits[structure->GetTeam()].Structures.AddUnique(structure);
		}
		else
		{
			InvalidUnits.Structures.Add(structure);
		}
	}
}

void ARTFPSGameState::OnMinionDeath(ARTSMinion* Minion)
{
	if (!Minion || !IsTeamValid(Minion->GetTeam())) return;

	if (AllUnits[Minion->GetTeam()].Minions.Remove(Minion) != (int32)1)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ARTFPSGameState::OnMinionDeath] Minion Removal Unsuccessful!"));
	}
}

void ARTFPSGameState::HandlePlayerDeath(AFPSServerController* Controller)
{

}

bool ARTFPSGameState::TeamInitialize(ADefaultMode* GameMode)
{
	bool result = Super::TeamInitialize(GameMode);
	if (result)
	{
		for (int i = 0; i < GameMode->GetNumTeams(); i++)
		{
			RTSTeamUnits newunitteam;
			AllUnits.Emplace(newunitteam);
		}
	}

	return false;
}




