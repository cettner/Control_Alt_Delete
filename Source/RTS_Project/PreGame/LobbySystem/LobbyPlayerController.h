// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GameArchitecture/Lobby/LobbyGameState.h"
#include "LobbyPlayerController.generated.h"

/**
 * 
 */

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

	/*Only used by Listen server*/
	void RefreshServerLobbyUI(TArray<FLobbyData> LobbyData);

};
