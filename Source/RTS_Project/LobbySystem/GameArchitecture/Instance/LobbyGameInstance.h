// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Interfaces/OnlineFriendsInterface.h"

#include "../../Interfaces/SessionMenuInterface.h"
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

USTRUCT(BlueprintType)
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

DECLARE_DELEGATE_OneParam(FFriendsListReadyDelegate, const TArray<FSubSytemFriendInfo>);

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
	FServerSettings GetServerSettings() const;

	TArray<FName> GetAvailableSubsystems() const;

	static FUniqueNetIdRepl GetUniquePlayerNetId(APlayerController* InPlayer);

	void StartOfflineGame();

	bool IsPlayingOffline();

	// Create menu called from the level blueprint
	UFUNCTION(BlueprintCallable)
	void LoadMainMenu();

	UFUNCTION(BlueprintCallable)
	void LoadLobbyMenu();

	//template <typename WidgetT = UUserWidget, typename OwnerT = UObject>
	ULobbyMenu * GetLobbyMenu();

	void StartGame();

	virtual void TravelToLobby();

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

//Friend Events
public:
	/*Call This to Poke the Given Subsytem and Request a Friends List, Request is Returned from FriendslistReadyDelegate Call if */
	void ReadFriendsList(FName SubSystemName);

	/*Bindable Delagate that is fired after the friends list for the selected SubSystem has been Retrieved*/
	FFriendsListReadyDelegate FriendsListReadyDelegate;

	/*Call this to send a invite through the default SubSystem to the designated player*/
	void SendSessionInviteToFriend(APlayerController* InvitingPlayer, const FBPUniqueNetId& FriendID);

protected:
	/*Callback Function when Subsystem Completes Request for Friends List*/
	void OnReadFriendsListCompleted(int32 LocalUserNum, bool bWasSuccessful, const FString & ListName, const FString & ErrorString);

	/*Function Called when Friend initally Accepts Invites, initiates Join Session Call*/
	void OnSessionUserInviteAccepted(bool bWasSuccessful, int32 LocalUserNum, TSharedPtr<const FUniqueNetId> InvitingPlayer, const FOnlineSessionSearchResult & TheSessionInvitedTo);
	
	/*"Pure" Virtual Function, Override Per Implemented Subsystem IE Steam, Epic...etc*/
	virtual UTexture2D * GetFriendAvatar(FBPUniqueNetId PlayerNetID, FName SubSystemName);

	/*Delegates for Above Events*/
	FOnReadFriendsListComplete ReadFriendListCompleteDelagate;
	FOnAcceptInviteComplete SessionInviteAcceptedDelegate;

	/*Last Called Subsystem through ReadFriendsList*/
	FName LastReadSubSystem;

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

protected:

	UPROPERTY(EditDefaultsOnly, Category = SubSystem)
	TArray<FName> IntegratedSubSystems;

	// Session
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

};
