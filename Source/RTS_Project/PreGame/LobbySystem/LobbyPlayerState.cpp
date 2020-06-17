// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyPlayerState.h"
#include "LobbyPlayerController.h"

void ALobbyPlayerState::ClientInitialize(AController* Controller)
{
	Super::ClientInitialize(Controller);
	ALobbyPlayerController * PC = Cast<ALobbyPlayerController>(Controller);
	if (PC)
	{
		PC->RefreshLobbyUI();
	}
}