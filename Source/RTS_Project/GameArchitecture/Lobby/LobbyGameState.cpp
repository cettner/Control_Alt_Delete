// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameState.h"
#include "UnrealNetwork.h"
#include "GameArchitecture/Instance/LobbyGameInstance.h"


bool ALobbyGameState::AddPlayertoLobby(APlayerController* NewPlayer)
{
	if (PlayersinLobby == MaxPlayers && LobbyData.Num() > 0) 	return false;

	int smallestteamindex = -1;
	int emptyslotindex = -1;
	int smallestteamsize = MaxPlayers;

	/*Figure out which team has the smallest number of players*/
	for (int i = 0; i < LobbyData.Num(); i++)
	{
		if (LobbyData[i].ActiveNum() < smallestteamsize)
		{
			smallestteamindex = i;
			smallestteamsize = LobbyData[i].ActiveNum();
		}
	}

	if (smallestteamindex < 0 || LobbyData[smallestteamindex].TeamData.Num() == 0) return false;

	/*Find an empty lobby slot in the smallest team*/
	for (int i = 0; i < LobbyData[smallestteamindex].TeamData.Num();)
	{
		if (LobbyData[smallestteamindex].TeamData[i].isSlotActive == false)
		{
			emptyslotindex = i;
			break;
		}
	}

	if (emptyslotindex < 0) return false;


	LobbyData[smallestteamindex].TeamData[emptyslotindex].isSlotActive = true;
	LobbyData[smallestteamindex].TeamData[emptyslotindex].OwningPlayerID = NewPlayer->GetUniqueID();
	PlayersinLobby++;
	return true;
}

ALobbyGameState::ALobbyGameState()
{
	LobbyData = TArray<FLobbyData>();
}

void ALobbyGameState::OnRep_LobbyInfo()
{
	int debug = 19;
}


TArray<FLobbyData> ALobbyGameState::GetLobbyData()
{
	return (LobbyData);
}

void ALobbyGameState::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (HasAuthority())
	{
		ULobbyGameInstance * GI = Cast<ULobbyGameInstance>(GetGameInstance());
		if (GI == nullptr) return;

		FLobbySettings initsettings = GI->GetLobbySettings();
		NumTeams = initsettings.NumTeams;
		NumPlayersPerTeam = initsettings.NumPlayersPerTeam;
		MaxPlayers = NumTeams * NumPlayersPerTeam;

		for (size_t i = 0; i < initsettings.NumTeams; i++)
		{
			FLobbyData teamslots = FLobbyData();
			for (size_t k = 0; k < initsettings.NumPlayersPerTeam; k++)
			{
				FSlotPlayerData tempslot;
				tempslot.isSlotActive = false;
				tempslot.SlotId = k;
				tempslot.TeamId = i;
				tempslot.PlayerName = "ERROR";
				
				teamslots.TeamData.Emplace(tempslot);
			}
			LobbyData.Emplace(teamslots);
		}
	}
}

void ALobbyGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ALobbyGameState, LobbyData);
}
