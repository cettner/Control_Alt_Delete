// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Interfaces/OnlineFriendsInterface.h"
#include "Blueprint/UserWidget.h"

#include "../../Interfaces/SessionMenuInterface.h"
#include "LobbyGameInstance.generated.h"




USTRUCT()
struct FLobbySettings
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly)
	int NumTeams = 2;

	UPROPERTY(EditDefaultsOnly)
	int NumPlayersPerTeam = 3;
};

USTRUCT()
struct FPlayerSettings
{
	GENERATED_USTRUCT_BODY()

	TSharedPtr<const FUniqueNetId> PlayerId = TSharedPtr<const FUniqueNetId>();

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

USTRUCT()
struct FBPUniqueNetId
{
	GENERATED_USTRUCT_BODY()

private:
	bool bUseDirectPointer;


public:
	TSharedPtr<const FUniqueNetId> UniqueNetId;
	const FUniqueNetId * UniqueNetIdPtr;

	void SetUniqueNetId(const TSharedPtr<const FUniqueNetId> &ID)
	{
		bUseDirectPointer = false;
		UniqueNetIdPtr = nullptr;
		UniqueNetId = ID;
	}

	void SetUniqueNetId(const FUniqueNetId *ID)
	{
		bUseDirectPointer = true;
		UniqueNetIdPtr = ID;
	}

	bool IsValid() const
	{
		if (bUseDirectPointer && UniqueNetIdPtr != nullptr)
		{
			return true;
		}
		else if (UniqueNetId.IsValid())
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	const FUniqueNetId* GetUniqueNetId() const
	{
		if (bUseDirectPointer && UniqueNetIdPtr != nullptr)
		{
			return (UniqueNetIdPtr);
		}
		else if (UniqueNetId.IsValid())
		{
			return UniqueNetId.Get();
		}
		else
			return nullptr;
	}

	FBPUniqueNetId()
	{
		bUseDirectPointer = false;
		UniqueNetIdPtr = nullptr;
	}
};

USTRUCT()
struct FSubSytemFriendInfo
{
	GENERATED_USTRUCT_BODY()

	UTexture2D* PlayerAvatar;

	FString PlayerName;

	FBPUniqueNetId PlayerUniqueNetID;
};

struct FStoredSessionSettings
{
	FString ServerName = "";
	FOnlineSessionSettings Settings = FOnlineSessionSettings();
	bool bIsValid = false;
};

DECLARE_DELEGATE_OneParam(FFriendsListReadyDelegate, const TArray<FSubSytemFriendInfo>);

/*Forward Declarations*/
class ULobbyMenu;


UCLASS()
class RTS_PROJECT_API ULobbyGameInstance : public UGameInstance, public ISessionMenuInterface
{
	GENERATED_BODY()
	
public:
	ULobbyGameInstance(const FObjectInitializer& ObjectInitializer);

	/*Called by server at lobby startup*/
	FLobbySettings GetLobbySettings();

	bool SetPlayerSettings(FPlayerSettings settings);

	FPlayerSettings GetPlayerSettings();

	bool SetServerSettings(FServerSettings settings);
	FServerSettings GetServerSettings() const;

	TArray<FName> GetAvailableSubsystems() const;

	FOnlineSessionSettings GetDefaultSessionSettings(FSessionSettings CustomSettings = FSessionSettings()) const;

	static FUniqueNetIdRepl GetUniquePlayerNetId(APlayerController* InPlayer);

	void StartOfflineGame();

	bool IsPlayingOffline();

	void LoadMainMenu();

	void LoadLobbyMenu();

	void CloseCurrentMenu();

	ULobbyMenu * GetLobbyMenu();

	void StartGame();

	virtual void TravelToLobby();


public:
	///// ISessionMenuInterface /////////////////// 
	void Host(const FString InServerName, const FSessionSettings CustomSettings = FSessionSettings()) override;

	void JoinSession(uint32 Index) override;

	void EndSession() override;

	void OpenSessionListMenu() override;
	///// ISessionMenuInterface /////////////////// 

protected:
	virtual void InitDefaultSessionSettings(FOnlineSessionSettings& OutSettings) const;
	virtual void InitCustomSessionSettings(FSessionSettings& OutSettings) const;
	virtual FOnlineSessionSettings GetHostSettings(FSessionSettings InAdditionalSettings) const;

/**************************************UGameInstance*********************************************/
protected:
	virtual void Init() override;
	virtual void PreloadContentForURL(FURL InURL) override;
	virtual void OnWorldChanged(UWorld * InOldWorld, UWorld * InNewWorld) override;
/*************************************************************************************************/
private:
	// Session Events
	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void OnFindSessionsComplete(bool Success);
	void OnJoinSessionsComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void CreateSession( const FOnlineSessionSettings SessionSettings, const FString InServerName);
	
	/*Settings are stored if a session needs a restart and cleared once a restart is requested*/
	void StoreSessionSettings(FStoredSessionSettings InSettings);
	void ClearStoredSettings();
	bool GetStoredSessionSettings(FStoredSessionSettings &OutSettings) const;
	bool AreSessionSettingsStored() const;
/**********************************************************************************************/

//Friend Events
public:
	/*Call This to Poke the Given Subsytem and Request a Friends List, Request is Returned from FriendslistReadyDelegate Call if */
	void ReadFriendsList(FName SubSystemName);

	/*Call this to send a invite through the default SubSystem to the designated player*/
	void SendSessionInviteToFriend(APlayerController* InvitingPlayer, const FBPUniqueNetId& FriendID);

protected:
	/*Callback Function when Subsystem Completes Request for Friends List*/
	void OnReadFriendsListCompleted(int32 LocalUserNum, bool bWasSuccessful, const FString & ListName, const FString & ErrorString);

	/*Function Called when Friend initally Accepts Invites, initiates Join Session Call*/
	void OnSessionUserInviteAccepted(bool bWasSuccessful, int32 LocalUserNum, TSharedPtr<const FUniqueNetId> InvitingPlayer, const FOnlineSessionSearchResult & TheSessionInvitedTo);
	
	/*"Pure" Virtual Function, Override Per Implemented Subsystem IE Steam, Epic...etc*/
	virtual UTexture2D * GetFriendAvatar(FBPUniqueNetId PlayerNetID, FName SubSystemName);


public:
	/*Bindable Delagate that is fired after the friends list for the selected SubSystem has been Retrieved*/
	FFriendsListReadyDelegate FriendsListReadyDelegate;
/**********************************************************************************************/
protected:
	// Main Menu
	UPROPERTY(EditDefaultsOnly, Category = UI)
	TSubclassOf<UUserWidget> MenuClass;
	UUserWidget* MainMenu;

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
	bool RestartSession;

	bool bIsPlayingOffline;

	FOnlineSessionSettings DefaultSessionSettings;

	FLobbySettings LobbySettings;

	FOnReadFriendsListComplete ReadFriendListCompleteDelagate;
	FOnAcceptInviteComplete SessionInviteAcceptedDelegate;

	/*Last Called Subsystem through ReadFriendsList*/
	FName LastReadSubSystem;

private:
	/*Stored SessionData for use between Session Resets*/
	FStoredSessionSettings StoredSessionSettings;

public:
	UPROPERTY(EditDefaultsOnly, Category = Settings)
	FString PlayerName;
/**********************************************************************************************/

	FPlayerSettings PlayerSettings;
	FServerSettings ServerSettings;

protected:

	UPROPERTY(EditDefaultsOnly, Category = SubSystem)
	TArray<FName> IntegratedSubSystems;

	// Session
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

};
