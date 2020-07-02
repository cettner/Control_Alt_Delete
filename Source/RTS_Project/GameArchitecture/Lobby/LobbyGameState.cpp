// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameState.h"
#include "UnrealNetwork.h"
#include "PreGame/LobbySystem/LobbyPlayerController.h"
#include "GameArchitecture/Instance/LobbyGameInstance.h"
#include "PreGame/LobbySystem/LobbyMenu.h"
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

	/*Refresh the lobby UI for listen servers, Clients Will draw during lobby info replication*/
	if (GetNetMode() == NM_ListenServer)
	{
		UWorld* World = GetWorld();
		if (World == nullptr) return false;
		ALobbyPlayerController * PC = World->GetFirstPlayerController<ALobbyPlayerController>();
		if (PC == nullptr) return false;
		PC->RefreshLobbyUI();
	}

	return true;
}

bool ALobbyGameState::RemovePlayerFromLobby(ALobbyPlayerController* LeavingPlayer)
{
	FSlotPlayerData settings;
	bool retval = false;

	if (FindPlayerinLobby(LeavingPlayer, settings))
	{
		/*Team and SlotID Remain the Same For Future PLayers*/
		LobbyData[settings.TeamId].TeamData[settings.SlotId].PlayerName = "";
		LobbyData[settings.TeamId].TeamData[settings.SlotId].isSlotActive = false;
		LobbyData[settings.TeamId].TeamData[settings.SlotId].OwningPlayerID = -1;
		retval = true;


		/*Refresh the lobby UI for listen servers, Clients Will draw during lobby info replication*/
		if (GetNetMode() == NM_ListenServer)
		{
			UWorld* World = GetWorld();
			if (World == nullptr) return false;
			ALobbyPlayerController* PC = World->GetFirstPlayerController<ALobbyPlayerController>();
			if (PC == nullptr) return false;
			PC->RefreshLobbyUI();
		}
	}

	return(retval);
}

bool ALobbyGameState::RequestStartGame(ALobbyPlayerController * RequestingPlayer)
{
	if (RequestingPlayer == nullptr) return false;

	if (CanPlayerStartGame(RequestingPlayer) == false) return(false);
	
	ULobbyGameInstance * GI = GetGameInstance<ULobbyGameInstance>();

	if (GI == nullptr) return(false);
	
	GI->StartGame();
	return(true);
}

bool ALobbyGameState::CanPlayerStartGame(ALobbyPlayerController * Player)
{
	if (HasAuthority() == false) return(false);

	if (GetNetMode() == NM_ListenServer)
	{
		UWorld * World = GetWorld();
		ALobbyPlayerController * LocalPC = World->GetFirstPlayerController<ALobbyPlayerController>();
		if (World == nullptr || LocalPC == nullptr) return false;

		if (LocalPC == Player)
		{
			return(true);
		}

	}
	else if (GetNetMode() == NM_DedicatedServer)
	{
		/*Currently Unimplemented*/
		return(false);
	}
	return false;
}

bool ALobbyGameState::StoreLobbyData()
{
	ULobbyGameInstance * GI = GetGameInstance<ULobbyGameInstance>();
	if (GI == nullptr) return(false);

	if (GetNetMode() > NM_ListenServer)
	{
		/*Client: Only Store Player Data*/
		return(StorePlayerData(GI));
	}
	else if (GetNetMode() == NM_DedicatedServer)
	{
		/*Dedicated Server Only Store Server Data*/
		return(StoreServerData(GI));
	}
	else if(GetNetMode() == NM_ListenServer)
	{
		/*Listen Server, Store Both Player And Server Data*/
		return(StoreServerData(GI) && StorePlayerData(GI));
	}
	else
	{
		return false;
	}
}

bool ALobbyGameState::StorePlayerData(ULobbyGameInstance * GI)
{
	UWorld * World = GetWorld();
	if (World == nullptr) return false;

	ALobbyPlayerController * Player = World->GetFirstPlayerController<ALobbyPlayerController>();
	if (Player == nullptr) return false;

	/*Find the player in the lobby*/
	FSlotPlayerData playerslot;
	if (FindPlayerinLobby(Player, playerslot) == false) return(false);
	
	FPlayerSettings settings;
	settings.PlayerId = playerslot.OwningPlayerID;
	settings.TeamId = playerslot.TeamId;
	settings.bIsValid = true;

	SetCustomPlayerSettings(settings, playerslot);
	return(GI->SetPlayerSettings(settings));
}

bool ALobbyGameState::StoreServerData(ULobbyGameInstance * GI)
{
	FServerSettings ServerSettings = FServerSettings();
	for (int i = 0; i < LobbyData.Num(); i++)
	{
		FLobbyData team = LobbyData[i];
		for (int k = 0; k < team.TeamData.Num(); k++)
		{
			FSlotPlayerData slotdata = team.TeamData[k];
			
			if (slotdata.isSlotActive)
			{
				FPlayerSettings activeplayer;
				activeplayer.PlayerId = slotdata.OwningPlayerID;
				activeplayer.TeamId = slotdata.TeamId;
				activeplayer.bIsValid = true;
				ServerSettings.settings.Emplace(activeplayer);
				ServerSettings.bIsValid = true;
			}
		}
	}

	return (GI->SetServerSettings(ServerSettings));
}

void ALobbyGameState::SetCustomPlayerSettings(FPlayerSettings& outsettings, const FSlotPlayerData StoredSlot)
{
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
	
	/*Store Our Data Locally in the Game Instance Then Draw the new Lobby*/
	StoreLobbyData();

	ULobbyMenu* Lobby = PC->GetLobbyMenu();
	if (Lobby == nullptr) return;
	Lobby->DrawLobbySlots(LobbyData);
}

bool ALobbyGameState::FindPlayerinLobby(ALobbyPlayerController * player, FSlotPlayerData& OutSlot)
{ 
	if (player == nullptr || player->PlayerState == nullptr) return false;
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
	PC->RefreshLobbyUI();

	
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
