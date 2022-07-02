// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTS_Project/PreGame/MenuPlayercontroller.h"
#include "LobbyGameState.h"
#include "LobbyPlayerController.generated.h"

/*Forward Declarations*/
class ULobbyMenu;

UCLASS()
class RTS_PROJECT_API ALobbyPlayerController : public AMenuPlayerController
{
	GENERATED_BODY()

	ULobbyMenu* LobbyMenu;

protected:
	virtual void PreInitializeComponents() override;

protected:
	ALobbyPlayerController(const FObjectInitializer& ObjectInitializer);

public:
	ULobbyMenu* GetLobbyMenu();

	FSlotPlayerData GetPlayerSlotInfo() const;

	void SetPlayerSlotInfo(FSlotPlayerData InSlotData);

	void RequestMoveSlot(FSlotPlayerData RequestedSlot);

	UFUNCTION(Server, reliable, WithValidation)
	void ServerRequestMoveSlot(FSlotPlayerData RequestedSlot);

	void RequestStartGame();

	UFUNCTION(Server, reliable, WithValidation)
	void ServerRequestStartGame();

	/*Only used by Listen server*/
	void RefreshLobbyUI();

protected:
	FSlotPlayerData PlayerSlotInfo;


};
