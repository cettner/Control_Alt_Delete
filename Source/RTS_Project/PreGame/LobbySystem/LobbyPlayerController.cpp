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

void ALobbyPlayerController::RefreshServerLobbyUI(TArray<FLobbyData> LobbyData)
{
	if (LobbyMenu == nullptr) return;
	LobbyMenu->DrawLobbySlots(LobbyData);
}
