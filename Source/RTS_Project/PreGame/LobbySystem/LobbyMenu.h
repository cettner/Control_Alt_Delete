// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "LobbyMenu.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API ULobbyMenu : public UUserWidget
{
	GENERATED_BODY()

	protected:
		uint8 PlayersPerTeam;

	protected:
		UPROPERTY(meta = (BindWidget))
		UPanelWidget* TeamAList;

		UPROPERTY(meta = (BindWidget))
		UPanelWidget* TeamBList;

		UPROPERTY(meta = (BindWidget))
		UButton* StartGameButton;
		
		UPROPERTY(meta = (BindWidget))
		UButton* LeaveLobbyButton;
};
