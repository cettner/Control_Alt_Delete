// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RTS_Project/GameArchitecture/Lobby/LobbyGameState.h"
#include "LobbyPlayerController.generated.h"

/*Forward Declarations*/
class ULobbyMenu;

UCLASS()
class RTS_PROJECT_API ALobbyPlayerController : public APlayerController
{
	GENERATED_BODY()

	ULobbyMenu* LobbyMenu;

public:
	ALobbyPlayerController(const FObjectInitializer& ObjectInitializer);

public:
	ULobbyMenu* GetLobbyMenu();
	FSlotPlayerData PlayerSlotInfo;
	bool SetLobbyMenu(ULobbyMenu * menu);

	void RequestMoveSlot(FSlotPlayerData RequestedSlot);

	UFUNCTION(Server, reliable, WithValidation)
	void ServerRequestMoveSlot(FSlotPlayerData RequestedSlot);

	void RequestStartGame();

	UFUNCTION(Server, reliable, WithValidation)
	void ServerRequestStartGame();

	/*Only used by Listen server*/
	void RefreshLobbyUI();


};
