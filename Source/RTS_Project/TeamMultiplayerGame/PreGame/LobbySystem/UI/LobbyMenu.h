// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Delegates/Delegate.h"

#include "LobbySlotWidget.h"
#include "LobbyMenu.generated.h"

/**
 * 
 */


UCLASS()
class RTS_PROJECT_API ULobbyMenu : public UUserWidget
{
	GENERATED_BODY()
		
		UFUNCTION()
		virtual void DrawLobbySlots(TArray<FLobbyData> TeamSlots);

	protected:
		virtual bool Initialize() override;
		virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	protected:
		virtual bool InitLobbyDataBinding();

	protected:
	    UFUNCTION()
		void OnPressedStartGameButton();

		UFUNCTION()
		void OnPressedLeaveLobbyButton();

		UFUNCTION()
	    virtual bool CanStartGame();

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

	protected:
		bool bIsLobbyDataBound = false;
};
