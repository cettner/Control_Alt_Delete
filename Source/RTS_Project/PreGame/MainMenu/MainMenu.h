// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PreGame/LobbySystem/SessionMenuInterface.h"
#include "PreGame/MainMenu/Joining/JoinSessionInfoWidget.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
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
class RTS_PROJECT_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void Setup(ISessionMenuInterface* MenuInterface);

	virtual void Teardown();

protected:
	virtual bool Initialize() override;

protected:

	// Menus
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
	UWidget* HostSessionMenuWidget;


	// Link to buttons
	UPROPERTY(meta = (BindWidget))
	UButton* NewSessionButton;

	UFUNCTION()
	void OnNewSessionPressed();

	UPROPERTY(meta = (BindWidget))
	UButton* JoinSessionButton;

	UFUNCTION()
	void OnJoinSessionPressed();


protected:
	ISessionMenuInterface* SessionMenuInterface;

////////////////////////////////////////////////////////////////////////////////////////////////////
protected:
	/*User Input TODO:: Make Setable from UI*/
	UPROPERTY(EditDefaultsOnly, Category = Session)
	FString DesiredServerName;

	UPROPERTY(EditDefaultsOnly, Category = Session)
	int PlayersPerTeam;

////////////////////////////////////////////////////////////////////////////////////////////////////

	//// JOIN SESSIONS ///////
protected:


	UPROPERTY(meta = (BindWidget))
	UWidget* SessionListMenuWidget;

	UPROPERTY(meta = (BindWidget))
	UPanelWidget* ScrollSessionList;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UJoinSessionInfoWidget> SessionRowClass;
	TOptional<uint32> SelectedScrollIndex;

	// Buttons

	UPROPERTY(meta = (BindWidget))
	UButton* CancelJoinSessionButton;

	UFUNCTION()
	void OnCancelJoinSession();

	UPROPERTY(meta = (BindWidget))
	UButton* JoinSelectedSessionButton;

	UFUNCTION()
	void OnJoinSelectedSession();


public:
	void InitializeSessionsList(TArray<FServerData> Data);
	void SelectIndexSessionList(uint32 Index);
	void UpdateSessionList();
	
};
