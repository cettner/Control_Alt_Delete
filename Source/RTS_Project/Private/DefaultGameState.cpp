// Fill out your copyright notice in the Description page of Project Settings.

#include "DefaultGameState.h"
#include "Engine/World.h"

bool ADefaultGameState::TeamInitialize(AGameMode * GameMode)
{
	if (HasAuthority())
	{
		GM = Cast<ADefaultMode>(GameMode);
		if (GM)
		{
			for (int i = 0; i < GM->GetNumTeams(); i++)
			{
				TArray<APlayerState *> newteam;
				Teams.Emplace(newteam);
			}
			initialized = GM->GetNumTeams() == Teams.Num();
		}	
	}
	return(initialized);
}

int ADefaultGameState::HasTeam(APlayerState * Player)
{
	bool found = false;
	int retval = -1;

	if (initialized)
	{
		for (int i = 0; i < GM->GetNumTeams(); i++)
		{
			found = Teams[i].Find(Player, retval);

			if (found)
			{
				break;
			}
		}
	}

	if (!found)
	{
		retval = -1;
	}

	return (retval);
}

bool ADefaultGameState::IsTeamFull(int Team_Index)
{
	bool retval = true;

	if (IsTeamValid(Team_Index) && initialized)
	{
		retval = Teams[Team_Index].Num() >= GM->GetTeamSize();
	}

	return (retval);
}

bool ADefaultGameState::IsTeamValid(int Team_Index)
{
	return(-1 < Team_Index && Team_Index < GM->GetNumTeams());
}

void ADefaultGameState::BeginPlay()
{
	Super::BeginPlay();
}

void ADefaultGameState::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

ADefaultGameState::ADefaultGameState(const FObjectInitializer & ObjectInitializer)
: Super(ObjectInitializer)
{
}

int ADefaultGameState::AssignAvailableTeam(APlayerState * New_Player)
{
	int retval = HasTeam(New_Player);

	if (retval > -1)
	{

	}
	else
	{
		for (int i = 0; i < GM->GetNumTeams(); i++)
		{
			if (Teams[i].Num() < GM->GetTeamSize())
			{
				Teams[i].AddUnique(New_Player);
				retval = i;
				break;
			}
		}
	}

	return(retval);
}

int ADefaultGameState::AssignBalancedTeam(APlayerState * New_Player)
{
	int retval = HasTeam(New_Player);

	if(retval > -1)
	{

	}
	else
	{
		int smallest_team_index = -1;
		int smallest_team_size = GM->GetTeamSize();

		for (int i = 0; i < GM->GetNumTeams(); i++)
		{
			if (Teams[i].Num() < smallest_team_size)
			{
				smallest_team_index = i;
				smallest_team_size = Teams[i].Num();
			}
		}

		if (smallest_team_size < GM->GetTeamSize())
		{
			Teams[smallest_team_index].AddUnique(New_Player);
			retval = smallest_team_index;
		}
	}
	
	return retval;
}


