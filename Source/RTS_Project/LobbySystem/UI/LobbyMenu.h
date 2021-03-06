// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "LobbySlotWidget.h"
#include "LobbyMenu.generated.h"

/**
 * 
 */


 // Struct for sessions



UCLASS()
class RTS_PROJECT_API ULobbyMenu : public UUserWidget
{
	GENERATED_BODY()
	public:
		virtual void Setup();
		virtual void Teardown();
		virtual void DrawLobbySlots(TArray<FLobbyData> TeamSlots);

	protected:
		virtual bool Initialize() override;

	protected:
	    UFUNCTION()
		void OnPressedStartGameButton();

		UFUNCTION()
		void OnPressedLeaveLobbyButton();

	protected:
		UPROPERTY(EditDefaultsOnly)
		TSubclassOf<ULobbySlotWidget> LobbySlotClass;

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
