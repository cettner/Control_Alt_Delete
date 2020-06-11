// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerController.h"

ALobbyPlayerController::ALobbyPlayerController(const FObjectInitializer& ObjectInitializer)
{
	PlayerSlotInfo.isSlotActive = false;
	PlayerSlotInfo.SlotId = -1;
	PlayerSlotInfo.TeamId = -1;
	PlayerSlotInfo.PlayerName = "";
}

ULobbyMenu* ALobbyPlayerController::GetLobbyMenu()
{
	return LobbyMenu;
}

bool ALobbyPlayerController::SetLobbyMenu(ULobbyMenu* menu)
{
	if (menu != nullptr)
	{
		LobbyMenu = menu;
	}

	return(LobbyMenu != nullptr);
}

void ALobbyPlayerController::RequestMoveSlot(FSlotPlayerData RequestedSlot)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		UWorld* World = GetWorld();
		if (World == nullptr) return;

		ALobbyGameState* GS = World->GetGameState<ALobbyGameState>();
		if (GS == nullptr) return;

		GS->ServerRequestMoveSlot(this,RequestedSlot);
	}
	else
	{
		ServerRequestMoveSlot(RequestedSlot);
	}
}

void ALobbyPlayerController::RefreshServerLobbyUI(TArray<FLobbyData> LobbyData)
{
	if (LobbyMenu == nullptr) return;
	LobbyMenu->DrawLobbySlots(LobbyData);
}

bool ALobbyPlayerController::ServerRequestMoveSlot_Validate(FSlotPlayerData RequestedSlot)
{
	return(true);
}

void ALobbyPlayerController::ServerRequestMoveSlot_Implementation (FSlotPlayerData RequestedSlotData)
{
	RequestMoveSlot(RequestedSlotData);
}