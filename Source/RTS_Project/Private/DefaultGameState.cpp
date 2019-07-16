// Fill out your copyright notice in the Description page of Project Settings.

#include "DefaultGameState.h"


bool ADefaultGameState::TeamInitialize()
{
	for (int i = 0; i < PlayerArray.Num(); i++)
	{

	}
	return(false);
}

int ADefaultGameState::HasTeam(APlayerState * Player)
{
	bool found = false;
	int retval = -1;
	for (int i = 0; i < GM->GetNumTeams(); i++)
	{
		found |= Teams[i].Find(Player, retval);
	}

	if (!found)
	{
		retval = -1;
	}
	return (retval);
}

ADefaultGameState::ADefaultGameState(const FObjectInitializer & ObjectInitializer)
: Super(ObjectInitializer)
{
	/*GM = Cast<ADefaultMode>(GetWorld()->GetAuthGameMode());
	initialized = TeamInitialize() && GM;

	FName State = "Lobby";
	SetMatchState(State);
	*/
}

int ADefaultGameState::AssignAvailableTeam(APlayerState * New_Player)
{
	for (int i = 0; i < GM->GetNumTeams(); i++)
	{
		for (int k = 0; k < GM->GetTeamSize(); k++)
		{
			if (Teams[i][k] == nullptr)
			{
				Teams[i][k] = New_Player;
				return(i);
			}
		}
	}
	return(-1);
}

void ADefaultGameState::OnRep_MatchState()
{
	Super::OnRep_MatchState();
}


