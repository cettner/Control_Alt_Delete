// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "LobbyGameState.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	ALobbyGameState* GS = GetGameState<ALobbyGameState>();
	if (NewPlayer == nullptr || GS == nullptr) return;
	if (!GS->AddPlayertoLobby(NewPlayer))
	{
		UE_LOG(LogTemp, Warning, TEXT("ALobbyGameMode:: Failed to Add player to lobby!"));
	}
}
