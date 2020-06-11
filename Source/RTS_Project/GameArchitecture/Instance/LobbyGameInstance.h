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




USTRUCT()
struct FLobbySettings
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly)
	int NumTeams = 2;

	UPROPERTY(EditDefaultsOnly)
	int NumPlayersPerTeam = 2;
};


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

public:

	virtual bool CanStartMatch();

	/*Called by server at lobby startup*/
	FLobbySettings GetLobbySettings();


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

	//Lobby
	UPROPERTY(EditDefaultsOnly, Category = Session)
	FString LobbyMapName;

protected:
	bool RestartSession;

/**********************************************************************************************/
protected:
	/*User Input TODO:: Make Setable from UI*/
	UPROPERTY(EditDefaultsOnly, Category = Session)
	FString DesiredServerName;

	FLobbySettings LobbySettings;

public:
	UPROPERTY(EditDefaultsOnly, Category = Session)
	FString PlayerName;
/**********************************************************************************************/

private:

	// Session
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

};
