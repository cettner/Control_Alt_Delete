// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "LobbyGameState.h"
#include "PreGame/LobbySystem/LobbyPlayerController.h"
#include "PreGame/MenuPawn.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	ALobbyGameState* GS = GetGameState<ALobbyGameState>();

	UWorld* World = GetWorld();
	if (World == nullptr) return;

	ALobbyPlayerController* PC = World->GetFirstPlayerController<ALobbyPlayerController>();
	if (PC == nullptr) return;

	if (PC == nullptr || GS == nullptr) return;
	int debug = PC->PlayerState->PlayerId;

	if (!GS->AddPlayertoLobby(PC))
	{
		UE_LOG(LogTemp, Warning, TEXT("ALobbyGameMode:: Failed to Add player to lobby!"));
	}

	/*If we're a listen server, draw the new lobby*/
	if (GetNetMode() == NM_ListenServer)
	{
		ULobbyMenu* Lobby = PC->GetLobbyMenu();
		if (Lobby == nullptr) return;

		Lobby->DrawLobbySlots(GS->GetLobbyData());
	}

}

ALobbyGameMode::ALobbyGameMode(const FObjectInitializer& ObjectInitializer)
{
	PlayerControllerClass = ALobbyPlayerController::StaticClass();
	DefaultPawnClass = AMenuPawn::StaticClass();
}
