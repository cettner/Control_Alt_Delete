// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "LobbyGameState.h"
#include "LobbyPlayerController.h"
#include "LobbyPlayerState.h"
#include "..\GameSession\LobbyGameSession.h"
#include "RTS_Project/PreGame/MenuPawn.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	ALobbyGameState* GS = GetGameState<ALobbyGameState>();
	ALobbyPlayerController* NewLobbyPlayer = Cast<ALobbyPlayerController>(NewPlayer);
	
	if (GS == nullptr || NewLobbyPlayer == nullptr) return;

	if (!GS->AddPlayertoLobby(NewLobbyPlayer))
	{
		UE_LOG(LogTemp, Warning, TEXT("ALobbyGameMode:: Failed to Add player to lobby!"));
	}
}

void ALobbyGameMode::Logout(AController * Exiting)
{
	Super::Logout(Exiting);
	ALobbyGameState* GS = GetGameState<ALobbyGameState>();
	ALobbyPlayerController* PC = Cast<ALobbyPlayerController>(Exiting);
	if (GS == nullptr || PC == nullptr) return;

	/*Players "leave" the lobby ahead of the server when the game is started, so dont remove them if the game is starting*/
	if (!GS->IsGameStarting())
	{
		if (!GS->RemovePlayerFromLobby(PC))
		{
			UE_LOG(LogTemp, Warning, TEXT("ALobbyGameMode::[Logout] Failed to Remove Exiting Player to lobby!"));
		}
	}
}

ALobbyGameMode::ALobbyGameMode(const FObjectInitializer& ObjectInitializer)
{
	PlayerControllerClass = ALobbyPlayerController::StaticClass();
	PlayerStateClass = ALobbyPlayerState::StaticClass();
	DefaultPawnClass = AMenuPawn::StaticClass();
	GameSessionClass = ALobbyGameSession::StaticClass();
	GameStateClass = ALobbyGameState::StaticClass();
}
