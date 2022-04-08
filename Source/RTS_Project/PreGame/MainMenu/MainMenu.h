// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"

#include "RTS_Project/PreGame/MainMenu/Joining/JoinSessionInfoWidget.h"
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
	virtual void Teardown();

protected:
	virtual bool Initialize() override;
	virtual void AddToScreen(ULocalPlayer* LocalPlayer, int32 ZOrder) override;

protected:

	// Menus
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
	UWidget* HostSessionMenuWidget;


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
