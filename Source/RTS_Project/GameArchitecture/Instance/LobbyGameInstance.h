// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionInterface.h"
#include "PreGame/LobbySystem/SessionMenuInterface.h"
#include "PreGame/MainMenu/MainMenu.h"
#include "PreGame/LobbySystem/LobbyMenu.h"
#include "LobbyGameInstance.generated.h"



/**
 * 
 */
UCLASS()
class RTS_PROJECT_API ULobbyGameInstance : public UGameInstance, public ISessionMenuInterface
{
	GENERATED_BODY()
	
public:
	ULobbyGameInstance(const FObjectInitializer& ObjectInitializer);

	virtual void Init();

	// Create menu called from the level blueprint
	UFUNCTION(BlueprintCallable)
	void LoadMainMenu();

	UFUNCTION(BlueprintCallable)
	void LoadLobbyMenu();

	virtual bool CanStartMatch();

public:
	///// ISessionMenuInterface /////////////////// 
	UFUNCTION()
	void Host(FString ServerName) override;

	UFUNCTION()
	void JoinSession(uint32 Index) override;

	UFUNCTION()
	void EndSession() override;

	UFUNCTION()
	void OpenSessionListMenu() override;
	///// ISessionMenuInterface /////////////////// 

private:
	// Session Events
	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void OnFindSessionsComplete(bool Success);
	void OnJoinSessionsComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	void CreateSession();

protected:
	// Main Menu
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UUserWidget> MenuClass;
	UMainMenu* MainMenu;


	// Main Menu
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UUserWidget> LobbyClass;
	ULobbyMenu* LobbyMenu;

protected:
	bool RestartSession;

protected:
	/*User Input TODO:: Make Setable from UI*/
	UPROPERTY(EditDefaultsOnly, Category = Session)
	FString DesiredServerName;

protected:
	//Lobby
	UPROPERTY(EditDefaultsOnly, Category = Session)
	FString LobbyMapName;

private:

	// Session
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

};
