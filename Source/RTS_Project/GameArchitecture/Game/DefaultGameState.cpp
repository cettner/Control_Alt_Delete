 // Fill out your copyright notice in the Description page of Project Settings.

#include "DefaultGameState.h"

#include "Engine/World.h"
#include "Net/UnrealNetwork.h"

bool ADefaultGameState::TeamInitialize(ADefaultMode * GameMode)
{
	if (HasAuthority())
	{
		check(GameMode);
		SetMaxTeamSize(GameMode->GetTeamSize());
		SetNumTeams(GameMode->GetNumTeams());

		for (int i = 0; i < GameMode->GetNumTeams(); i++)
		{
			TArray<APlayerState *> newteam;
			Teams.Emplace(newteam);
		}
		initialized = GameMode->GetNumTeams() == Teams.Num();
	}

	return(initialized);
}

int ADefaultGameState::HasTeam(APlayerState * Player) const
{
	bool found = false;
	int retval = -1;

	if (initialized)
	{
		for (int i = 0; i < Teams.Num(); i++)
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

bool ADefaultGameState::IsTeamFull(int Team_Index) const
{
	bool retval = true;

	if (IsTeamValid(Team_Index) && initialized)
	{
		retval = Teams[Team_Index].Num() >= MaxTeamSize;
	}

	return (retval);
}

void ADefaultGameState::SetMaxTeamSize(int8 InTeamSize)
{
	MaxTeamSize = InTeamSize;
}

void ADefaultGameState::SetNumTeams(int8 InNumTeams)
{
	NumTeams = InNumTeams;
}

void ADefaultGameState::OnRep_ReplicatedHasBegunPlay()
{
	Super::OnRep_ReplicatedHasBegunPlay();
	
	const UWorld * world = GetWorld();
	ADefaultPlayerController* firstcontroller = world->GetFirstPlayerController<ADefaultPlayerController>();
	firstcontroller->OnMatchStart();
}

void ADefaultGameState::ReceivedGameModeClass()
{
	Super::ReceivedGameModeClass();

	if (!HasAuthority())
	{
		const UWorld* world = GetWorld();
		ADefaultPlayerController* firstcontroller = world->GetFirstPlayerController<ADefaultPlayerController>();

		firstcontroller->ClientInitUI();

		ADefaultPlayerState * ps = firstcontroller->GetPlayerState<ADefaultPlayerState>();
		if (ps != nullptr && !ps->HasDefaultGameModeLoaded())
		{
			ps->LoadGameModeDefaults(GetDefaultGameMode());
		}
	}
}

bool ADefaultGameState::IsTeamValid(int Team_Index) const
{
	return((Team_Index > -1) && (Team_Index < MaxTeamSize));
}

void ADefaultGameState::PlayerGameDataInit(APlayerState * Player)
{
	/*Pure Virtual Function, Map related data to populate the playerstate on login goes here*/
}

int ADefaultGameState::AssignAvailableTeam(APlayerState * New_Player)
{
	int retval = HasTeam(New_Player);

	if (retval > -1)
	{

	}
	else
	{
		for (int i = 0; i < NumTeams; i++)
		{
			if (Teams[i].Num() < MaxTeamSize)
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
		int smallest_team_size = MaxTeamSize;

		for (int i = 0; i < NumTeams; i++)
		{
			if (Teams[i].Num() < smallest_team_size)
			{
				smallest_team_index = i;
				smallest_team_size = Teams[i].Num();
			}
		}

		if (smallest_team_size < MaxTeamSize)
		{
			Teams[smallest_team_index].AddUnique(New_Player);
			retval = smallest_team_index;
		}
	}
	
	return retval;
}

void ADefaultGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(ADefaultGameState, MaxTeamSize, COND_InitialOnly);
	DOREPLIFETIME_CONDITION(ADefaultGameState, NumTeams, COND_InitialOnly);


}