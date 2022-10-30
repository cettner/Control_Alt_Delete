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
		const TSubclassOf<ATeamState> teamstateclass = GameMode->GetTeamStateClass();
		UWorld * world = GetWorld();

		for (int i = 0; i < GameMode->GetNumTeams(); i++)
		{
			ATeamState* newteam = world->SpawnActor<ATeamState>(teamstateclass);
			newteam->SetTeam(i);
			newteam->LoadServerDefaults(GameMode);
			TeamStates.Emplace(newteam);
		}
		initialized = GameMode->GetNumTeams() == TeamStates.Num();
	}

	return(initialized);
}

ATeamState * ADefaultGameState::GetTeamForPlayer(ADefaultPlayerState* Player) const
{
	ATeamState* retval = nullptr;

	if (!HasAuthority())
	{
		ATeamState* defaultteam = GetDefaultTeamState();
		if (IsValid(defaultteam) && defaultteam->IsPlayerOnTeam(Player))
		{
			retval = defaultteam;
		}
	}
	else
	{
		for (int i = 0; i < TeamStates.Num(); i++)
		{
			const bool found = TeamStates[i]->IsPlayerOnTeam(Player);

			if (found == true)
			{
				retval = TeamStates[i];
				break;
			}
		}
	}

	return (retval);
}

ATeamState* ADefaultGameState::GetDefaultTeamState() const
{
	ATeamState* retval = nullptr;

	const UWorld* world = GetWorld();
	const ADefaultPlayerController* pc = world->GetFirstPlayerController<ADefaultPlayerController>();

	if (pc == nullptr) return retval;
	const int teamid = pc->GetTeamID();

	return GetTeamState(teamid);
}

ATeamState* ADefaultGameState::GetTeamState(const int teamid) const
{
	ATeamState* retval = nullptr;
	for (int i = 0; i < TeamStates.Num(); i++)
	{
		if (TeamStates[i]->GetTeam() == teamid)
		{
			retval = TeamStates[i];
		}
	}
	return retval;
}

bool ADefaultGameState::IsTeamFull(int Team_Index) const
{
	bool retval = true;

	if (IsTeamValid(Team_Index))
	{
		retval = TeamStates[Team_Index]->GetCurrentPlayerNum() >= MaxTeamSize;
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

void ADefaultGameState::OnLocalTeamStateRecieved(ATeamState* InState)
{
	/*Todo:Add protection / sync if GameState is replicated after Teamstate for some reason*/
	/*Todo: Currently not called on listen server, but not sure if that's a use case that would ever be needed since server has all teamstates already*/
	TeamStates.AddUnique(InState);
}

void ADefaultGameState::OnRep_ReplicatedHasBegunPlay()
{
	Super::OnRep_ReplicatedHasBegunPlay();
}

void ADefaultGameState::ReceivedGameModeClass()
{
	Super::ReceivedGameModeClass();

	if (!HasAuthority())
	{
		const UWorld* world = GetWorld();
		ADefaultPlayerController* firstcontroller = world->GetFirstPlayerController<ADefaultPlayerController>();
		ADefaultPlayerState * ps = firstcontroller->GetPlayerState<ADefaultPlayerState>();

		/*if this fails, it is handeled when the playerstate replicates in ADefaultPlayerState::ClientInitialize*/
		if (ps != nullptr && !ps->HasDefaultGameModeLoaded())
		{
			ps->LoadGameModeDefaults(GetDefaultGameMode());
			firstcontroller->ClientInitUI();
		}
	}
}

bool ADefaultGameState::IsTeamValid(int Team_Index) const
{
	return((Team_Index > -1) && (Team_Index < MaxTeamSize));
}

void ADefaultGameState::PlayerGameDataInit(ADefaultPlayerState* Player)
{
	/*Pure Virtual Function, Map related data to populate the playerstate on login goes here*/
}

ATeamState* ADefaultGameState::AssignTeam(ADefaultPlayerState* Player, const int InRequestedTeamId)
{
	ATeamState* retval = nullptr;
	if (IsTeamValid(InRequestedTeamId)  && HasAuthority())
	{
		const bool success = TeamStates[InRequestedTeamId]->AddPlayer(Player);
		if (success == true)
		{
			retval = TeamStates[InRequestedTeamId];
		}
	}
	return retval;
}

ATeamState* ADefaultGameState::AssignAvailableTeam(ADefaultPlayerState* New_Player)
{
	ATeamState * foundteam = GetTeamForPlayer(New_Player);

	if( foundteam == nullptr)
	{
		for (int i = 0; i < NumTeams; i++)
		{
			if (TeamStates[i]->GetCurrentPlayerNum() < MaxTeamSize)
			{
				TeamStates[i]->AddPlayer(New_Player);
				foundteam = TeamStates[i];
				break;
			}
		}
	}

	return(foundteam);
}

ATeamState* ADefaultGameState::AssignBalancedTeam(ADefaultPlayerState * InNewPlayer)
{
	ATeamState* retval = GetTeamForPlayer(InNewPlayer);

	if(retval == nullptr)
	{
		int smallestteamindex = -1;
		int smallestteamsize = MaxTeamSize + 1;
		/*Find the smallest tean size*/
		for (int i = 0; i < NumTeams; i++)
		{
			const int32 currentteamsize = TeamStates[i]->GetCurrentPlayerNum();
			if (currentteamsize < smallestteamsize)
			{
				smallestteamindex = i;
				smallestteamsize = currentteamsize;
			}
		}

		if (smallestteamsize <= MaxTeamSize)
		{
			TeamStates[smallestteamindex]->AddPlayer(InNewPlayer);
			retval = TeamStates[smallestteamindex];
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