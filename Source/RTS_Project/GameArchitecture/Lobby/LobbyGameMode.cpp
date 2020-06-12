// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "LobbyGameState.h"
#include "PreGame/LobbySystem/LobbyPlayerController.h"
#include "LobbyGameSession.h"
#include "PreGame/MenuPawn.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	ALobbyGameState* GS = GetGameState<ALobbyGameState>();
	ALobbyPlayerController* NewLobbyPlayer = Cast<ALobbyPlayerController>(NewPlayer);
	UWorld* World = GetWorld();
	
	if (World == nullptr || GS == nullptr || NewLobbyPlayer == nullptr) return;

	if (!GS->AddPlayertoLobby(NewLobbyPlayer))
	{
		UE_LOG(LogTemp, Warning, TEXT("ALobbyGameMode:: Failed to Add player to lobby!"));
	}

	/*If we're a listen server, draw the new lobby*/
	if (GetNetMode() == NM_ListenServer)
	{
		ALobbyPlayerController* LocalController = World->GetFirstPlayerController<ALobbyPlayerController>();
		if (LocalController == nullptr) return;
		ULobbyMenu* Lobby = LocalController->GetLobbyMenu();
		if (Lobby == nullptr) return;
		Lobby->DrawLobbySlots(GS->GetLobbyData());
	}

}

ALobbyGameMode::ALobbyGameMode(const FObjectInitializer& ObjectInitializer)
{
	PlayerControllerClass = ALobbyPlayerController::StaticClass();
	DefaultPawnClass = AMenuPawn::StaticClass();
	GameSessionClass = ALobbyGameSession::StaticClass();
}
