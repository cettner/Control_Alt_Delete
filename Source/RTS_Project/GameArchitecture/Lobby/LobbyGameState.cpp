// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameState.h"
#include "UnrealNetwork.h"
#include "PreGame/LobbySystem/LobbyPlayerController.h"
#include "GameArchitecture/Instance/LobbyGameInstance.h"
#include "GameFramework/PlayerState.h"


bool ALobbyGameState::AddPlayertoLobby(ALobbyPlayerController* NewPlayer)
{
	if ( (NewPlayer == nullptr || PlayersinLobby == MaxPlayers) && LobbyData.Num() > 0) 	return false;

	/*Make Sure Player isnt already in the lobby*/
	FSlotPlayerData temp;
	if (FindPlayerinLobby(NewPlayer, temp) == true) return false;
	
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
	LobbyData[smallestteamindex].TeamData[emptyslotindex].OwningPlayerID = NewPlayer->PlayerState->PlayerId;
	LobbyData[smallestteamindex].TeamData[emptyslotindex].PlayerName = NewPlayer->PlayerState->GetPlayerName();

	NewPlayer->PlayerSlotInfo = LobbyData[smallestteamindex].TeamData[emptyslotindex];

	PlayersinLobby++;

	return true;
}

ALobbyGameState::ALobbyGameState()
{
	LobbyData = TArray<FLobbyData>();
}

void ALobbyGameState::OnRep_LobbyInfo()
{
	UWorld * World = GetWorld();
	if (World == nullptr) return;
	
	ALobbyPlayerController * PC = World->GetFirstPlayerController<ALobbyPlayerController>();
	if (PC == nullptr) return;

	ULobbyMenu* Lobby = PC->GetLobbyMenu();
	if (Lobby == nullptr) return;

	Lobby->DrawLobbySlots(LobbyData);
}

bool ALobbyGameState::FindPlayerinLobby(ALobbyPlayerController * player, FSlotPlayerData& OutSlot)
{
	int idtosearch = player->PlayerState->PlayerId;

	for (int i = 0; i < LobbyData.Num(); i++)
	{
		FLobbyData teamdata = LobbyData[i];
		for (int k = 0; k < teamdata.TeamData.Num(); k++)
		{
			FSlotPlayerData data = teamdata.TeamData[k];
			if (data.OwningPlayerID == idtosearch)
			{
				OutSlot = data;
				return true;
			}
		}
	}
	return false;
}

TArray<FLobbyData> ALobbyGameState::GetLobbyData()
{
	return (LobbyData);
}

bool ALobbyGameState::ServerRequestMoveSlot_Validate(ALobbyPlayerController * RequestingPlayer, FSlotPlayerData RequestedSlot)
{
	return(true);
}

void ALobbyGameState::ServerRequestMoveSlot_Implementation(ALobbyPlayerController * RequestingPlayer, FSlotPlayerData RequestedSlotData)
{
	if (RequestingPlayer == nullptr) return;
	/*Check if the RequestedSlot is Available*/
	int requestedteamid = RequestedSlotData.TeamId;
	int requestedslotid = RequestedSlotData.SlotId;
	if (LobbyData[requestedteamid].TeamData[requestedslotid].isSlotActive == true) return;


	/*Get the Slot the player currently occupies*/
	FSlotPlayerData currentslotdata;
	if (FindPlayerinLobby(RequestingPlayer, currentslotdata) == false) return;
	int currentteam = currentslotdata.TeamId;
	int currentslot = currentslotdata.SlotId;
	if (currentteam < 0 || currentslot < 0) return;

	FSlotPlayerData ownedslot = LobbyData[currentteam].TeamData[currentslot];

	/*Deativate the owned slot*/
	LobbyData[currentteam].TeamData[currentslot].isSlotActive = false;
	LobbyData[currentteam].TeamData[currentslot].PlayerName = "";
	LobbyData[currentteam].TeamData[currentslot].OwningPlayerID = 0xFFFFU;

	/*Put the player in its new spot*/
	LobbyData[requestedteamid].TeamData[requestedslotid].isSlotActive = true;
	LobbyData[requestedteamid].TeamData[requestedslotid].PlayerName = RequestingPlayer->PlayerState->GetPlayerName();
	LobbyData[requestedteamid].TeamData[requestedslotid].OwningPlayerID = RequestingPlayer->PlayerState->PlayerId;
	
	/*Store its new location*/
	RequestingPlayer->PlayerSlotInfo = LobbyData[requestedteamid].TeamData[requestedslotid];
	
	/*Refresh the lobby UI for listen servers*/
	UWorld* World = GetWorld();
	if (World == nullptr) return;
	ALobbyPlayerController * PC = World->GetFirstPlayerController<ALobbyPlayerController>();
	if (PC == nullptr) return;
	PC->RefreshServerLobbyUI(LobbyData);

	
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

		for (int i = 0; i < initsettings.NumTeams; i++)
		{
			FLobbyData teamslots = FLobbyData();
			for (int k = 0; k < initsettings.NumPlayersPerTeam; k++)
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
