// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "../../PreGame/LobbySystem/SessionMenuInterface.h"
#include "Interfaces/OnlineSessionInterface.h"
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

USTRUCT()
struct FPlayerSettings
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly)
	int PlayerId = -1;

	UPROPERTY(EditDefaultsOnly)
	int TeamId = -1;

	UPROPERTY(EditDefaultsOnly)
	bool bIsValid = false;

	UPROPERTY(EditDefaultsOnly)
	TMap<FString, int> CustomSettings = TMap<FString,int>();
};

USTRUCT()
struct FServerSettings
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly)
	TArray<FPlayerSettings> settings = TArray<FPlayerSettings>();

	UPROPERTY(EditDefaultsOnly)
	int NumTeams = 2;

	UPROPERTY(EditDefaultsOnly)
	int NumPlayersPerTeam = 2;

	UPROPERTY(EditDefaultsOnly)
	bool bIsValid = false;
};
	

/*Forward Declarations*/
class UMainMenu;
class ULobbyMenu;


UCLASS()
class RTS_PROJECT_API ULobbyGameInstance : public UGameInstance, public ISessionMenuInterface
{
	GENERATED_BODY()
	
public:
	ULobbyGameInstance(const FObjectInitializer& ObjectInitializer);

	virtual void Init();

	/*Called by server at lobby startup*/
	FLobbySettings GetLobbySettings();

	bool SetPlayerSettings(FPlayerSettings settings);
	FPlayerSettings GetPlayerSettings();

	bool SetServerSettings(FServerSettings settings);
	FServerSettings GetServerSettings();

	void StartOfflineGame();

	bool IsPlayingOffline();

	// Create menu called from the level blueprint
	UFUNCTION(BlueprintCallable)
	void LoadMainMenu();

	UFUNCTION(BlueprintCallable)
	void LoadLobbyMenu();

	void StartGame();

protected:
	bool RestartSession;

	bool bIsPlayingOffline;

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


/**********************************************************************************************/
protected:
	// Main Menu
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UUserWidget> MenuClass;
	UMainMenu* MainMenu;

	//Lobby
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UUserWidget> LobbyClass;
	ULobbyMenu* LobbyMenu;

/***********************************************************************************************/
	//Maps
	UPROPERTY(EditDefaultsOnly, Category = Maps)
	FString MainMenuMapName;

	UPROPERTY(EditDefaultsOnly, Category = Maps)
	FString LobbyMapName;

	UPROPERTY(EditDefaultsOnly, Category = Maps)
	FString GameMapName;
/**********************************************************************************************/

protected:
	/*User Input TODO:: Make Setable from UI*/
	UPROPERTY(EditDefaultsOnly, Category = Settings)
	FString DesiredServerName;

	FLobbySettings LobbySettings;

public:
	UPROPERTY(EditDefaultsOnly, Category = Settings)
	FString PlayerName;
/**********************************************************************************************/

	FPlayerSettings PlayerSettings;
	FServerSettings ServerSettings;

private:

	// Session
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

};
