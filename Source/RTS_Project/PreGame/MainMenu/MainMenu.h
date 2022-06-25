// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/Button.h"
#include "MenuWidgetBase.h"
#include "MainMenu.generated.h"


// Struct for sessions
USTRUCT()
struct FServerData
{
	GENERATED_BODY()

	FString Name;
	uint16 CurrentPlayers;
	uint16 MaxPlayers;
	FString HostUsername;
};



/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UMainMenu : public UMenuWidgetBase
{
	GENERATED_BODY()

protected:
	virtual void InitMenuBindings() override;

protected:
	virtual bool Initialize() override;
	virtual void AddToScreen(ULocalPlayer* LocalPlayer, int32 ZOrder) override;

protected:

	// Menus
	UPROPERTY(meta = (BindWidget))
	UButton* GameSettingsButton;

	UPROPERTY(meta = (BindWidget))
	UButton* ExitGameButton;

	UPROPERTY(meta = (BindWidget))
	UButton* MultiPlayerButton;


////////////////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////////////////

	//// JOIN SESSIONS ///////

/*
protected:
	UPROPERTY(meta = (BindWidget))
	UWidget* SessionListMenuWidget;

	UPROPERTY(meta = (BindWidget))
	UPanelWidget* ScrollSessionList;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UJoinSessionInfoWidget> SessionRowClass;
	TOptional<uint32> SelectedScrollIndex;

	// Buttons
	UFUNCTION()
	void OnCancelJoinSession();

	UPROPERTY(meta = (BindWidget))
	UButton* CancelJoinSessionButton;


	UPROPERTY(meta = (BindWidget))
	UButton* JoinSelectedSessionButton;

	UFUNCTION()
	void OnJoinSelectedSession();


public:
	void InitializeSessionsList(TArray<FServerData> Data);
	void SelectIndexSessionList(uint32 Index);
	void UpdateSessionList();
*/
};
