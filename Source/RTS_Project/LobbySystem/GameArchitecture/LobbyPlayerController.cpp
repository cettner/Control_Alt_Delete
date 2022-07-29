// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerController.h"
#include "../UI/LobbyMenu.h"


ALobbyPlayerController::ALobbyPlayerController(const FObjectInitializer& ObjectInitializer)
{
	PlayerSlotInfo.isSlotActive = false;
	PlayerSlotInfo.SlotId = -1;
	PlayerSlotInfo.TeamId = -1;
	PlayerSlotInfo.PlayerName = "";
}

FSlotPlayerData ALobbyPlayerController::GetPlayerSlotInfo() const
{
	return PlayerSlotInfo;
}

void ALobbyPlayerController::SetPlayerSlotInfo(FSlotPlayerData InSlotData)
{
	PlayerSlotInfo = InSlotData;
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

void ALobbyPlayerController::RequestStartGame()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		UWorld* World = GetWorld();
		if (World == nullptr) return;

		ALobbyGameState* GS = World->GetGameState<ALobbyGameState>();
		if (GS == nullptr) return;

		GS->RequestStartGame(this);
	}
	else
	{
		ServerRequestStartGame();
	}
}

bool ALobbyPlayerController::ServerRequestMoveSlot_Validate(FSlotPlayerData RequestedSlot)
{
	return(true);
}

void ALobbyPlayerController::ServerRequestMoveSlot_Implementation (FSlotPlayerData RequestedSlotData)
{
	RequestMoveSlot(RequestedSlotData);
}

bool ALobbyPlayerController::ServerRequestStartGame_Validate()
{
	return(true);
}

void  ALobbyPlayerController::ServerRequestStartGame_Implementation()
{
	RequestStartGame();
}