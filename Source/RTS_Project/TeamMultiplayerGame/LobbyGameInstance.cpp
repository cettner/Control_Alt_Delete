 // Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameInstance.h"
#include "OnlineSubsystemTypes.h"
#include "Kismet/GameplayStatics.h"


const static FName SESSION_NAME = TEXT("RTSFPSGameSession");


ULobbyGameInstance::ULobbyGameInstance(const FObjectInitializer& ObjectInitializer)
{
	RestartSession = false;
	bIsPlayingOffline = false;
	PlayerName = "";
	LobbyMapName = "/Game/Maps/LobbyMap";
	GameMapName = "/Game/Maps/DevMap";
	MainMenuMapName = "/Game/Maps/MainMenuMap";
}

void ULobbyGameInstance::Init()
{
	Super::Init();
	IOnlineSubsystem* SubSystem = IOnlineSubsystem::Get();

	if (SubSystem != nullptr)
	{
		SessionInterface = SubSystem->GetSessionInterface();

		if (SessionInterface.IsValid())
		{
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &ULobbyGameInstance::OnCreateSessionComplete);

			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &ULobbyGameInstance::OnDestroySessionComplete);

			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &ULobbyGameInstance::OnFindSessionsComplete);

			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &ULobbyGameInstance::OnJoinSessionsComplete);

			SessionInterface->OnSessionUserInviteAcceptedDelegates.AddUObject(this, &ULobbyGameInstance::OnSessionUserInviteAccepted);

			ReadFriendListCompleteDelagate = FOnReadFriendsListComplete::CreateUObject(this, &ULobbyGameInstance::OnReadFriendsListCompleted);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[ULobbyGameInstance::Init] Found NO SUBSYSTEM"));
	}
	
	InitDefaultSessionSettings(DefaultSessionSettings);
}

void ULobbyGameInstance::PreloadContentForURL(FURL InURL)
{
	Super::PreloadContentForURL(InURL);
	StartLoadingLevel(InURL);
}

void ULobbyGameInstance::OnWorldChanged(UWorld* InOldWorld, UWorld* InNewWorld)
{
	Super::OnWorldChanged(InOldWorld, InNewWorld);
}

void ULobbyGameInstance::LoadComplete(const float LoadTime, const FString& MapName)
{
	ULoadingWidget * loadscreen = GetLoadingWidget();

	if (IsValid(loadscreen))
	{
		loadscreen->OnLevelLoadComplete();
	}
}

void ULobbyGameInstance::StartLoadingLevel(FURL InURL, ETravelType TravelType, bool bIsSeamlessTravel)
{
	if (!IsLoadingLevel())
	{
		ULoadingWidget* loadingmenu = CreateWidget<ULoadingWidget>(this, OfflineLoadMenuClass);
		loadingmenu->SetTransitionURL(InURL, this);
		SetLoadingScreen(loadingmenu);
	}
}

bool ULobbyGameInstance::IsLoadingLevel() const
{
	bool retval = false;
	const ULoadingWidget* loadmenu = GetLoadingWidget();
	
	if (IsValid(loadmenu) && loadmenu->IsLoading())
	{
		retval = true;
	}

	return retval;
}

void ULobbyGameInstance::OnLoadLevelComplete(FURL InLoadDestination, bool InLoadSuccess)
{
	CloseLoadingScreen();
}

void ULobbyGameInstance::CloseLoadingScreen()
{
		if (IsValid(LoadingScreen))
		{
			UGameViewportClient* viewport = GetGameViewportClient();
			viewport->RemoveViewportWidgetContent(LoadingScreen->TakeWidget());
		}
		LoadingScreen = nullptr;
}

void ULobbyGameInstance::SetLoadingScreen(ULoadingWidget* InMenu)
{
	LoadingScreen = InMenu;
}

void ULobbyGameInstance::StartGame()
{
	UWorld* World = GetWorld();

	const FString modifier = "?listen";
	World->ServerTravel(GameMapName + modifier);
}

void ULobbyGameInstance::TravelToLobby()
{
	UWorld* World = GetWorld();

	const FString modifier = "?listen";
	World->ServerTravel(LobbyMapName + modifier);
}

FLobbySettings ULobbyGameInstance::GetLobbySettings()
{
	return LobbySettings;
}

bool ULobbyGameInstance::SetPlayerSettings(FPlayerSettings settings)
{
	PlayerSettings = settings;
	return true;
}

FPlayerSettings ULobbyGameInstance::GetPlayerSettings() const
{
	return PlayerSettings;
}

bool ULobbyGameInstance::SetServerSettings(FServerSettings settings)
{
	ServerSettings = settings;
	return true;
}

FServerSettings ULobbyGameInstance::GetServerSettings() const
{
	return(ServerSettings);
}

TArray<FName> ULobbyGameInstance::GetAvailableSubsystems() const
{
	return IntegratedSubSystems;
}

FOnlineSessionSettings ULobbyGameInstance::GetDefaultSessionSettings(FSessionSettings CustomSettings) const
{
	FOnlineSessionSettings retval = DefaultSessionSettings;
	retval.Settings = CustomSettings;
	return retval;
}

FUniqueNetIdRepl ULobbyGameInstance::GetUniquePlayerNetId(APlayerController* PlayerController)
{
	FUniqueNetIdRepl UniqueNetIdRepl;

	if (PlayerController->IsLocalPlayerController())
	{
		ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
		if (IsValid(LocalPlayer))
		{
			UniqueNetIdRepl = LocalPlayer->GetPreferredUniqueNetId();
		}
		else
		{
			UNetConnection* RemoteNetConnection = Cast<UNetConnection>(PlayerController->Player);
			check(IsValid(RemoteNetConnection));
			UniqueNetIdRepl = RemoteNetConnection->PlayerId;
		}
	}
	else
	{
		UNetConnection* RemoteNetConnection = Cast<UNetConnection>(PlayerController->Player);
		check(IsValid(RemoteNetConnection));
		UniqueNetIdRepl = RemoteNetConnection->PlayerId;
	}

	return UniqueNetIdRepl;
}

void ULobbyGameInstance::StartOfflineGame()
{
	const UWorld* World = GetWorld();
	if (World)
	{
		CloseLoadingScreen();

		bIsPlayingOffline = true;
		UGameplayStatics::OpenLevel(World, FName(*LobbyMapName));
	}
}

bool ULobbyGameInstance::IsPlayingOffline() const
{
	return bIsPlayingOffline;
}

void ULobbyGameInstance::Host(const FString InServerName, const FSessionSettings CustomSettings)
{

	if (SessionInterface.IsValid())
	{
		// Checks for an existing session
		FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);

		if (ExistingSession != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("[ULobbyGameInstance::Host] There is an existing session about to remove the current one"));
			RestartSession = true;

			FStoredSessionSettings settingstostore;
			settingstostore.ServerName = InServerName;
			settingstostore.Settings = GetDefaultSessionSettings(CustomSettings);
			settingstostore.bIsValid = true;
			StoreSessionSettings(settingstostore);

			SessionInterface->DestroySession(SESSION_NAME);
		}
		else
		{
			// Create a new session
			const FOnlineSessionSettings sessionsettings = GetDefaultSessionSettings(CustomSettings);
			CreateSession(sessionsettings, InServerName);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[ULobbyGameInstance::Host] SessionInterface invalid"));
	}
}

void ULobbyGameInstance::JoinSession(uint32 Index)
{
	ULocalPlayer* const Player = GetFirstGamePlayer();

	if (!SessionInterface.IsValid() || (!SessionSearch.IsValid())  || !Player) return;

	if (Index < (uint32)(SessionSearch->SearchResults.Num()))
	{
		SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);
	}
}

void ULobbyGameInstance::BeginSearchQuery()
{

	SessionSearch = MakeShareable(new FOnlineSessionSearch());

	if (SessionSearch.IsValid())
	{
		SessionSearch->bIsLanQuery = DefaultSessionSettings.bIsLANMatch;
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[ULobbyGameInstance::BeginSearchQuery] Session is Invalid"));
	}
}

bool ULobbyGameInstance::IsSearchingSession(const FName SessionName)
{
	bool retval = false;
	if (SessionSearch.IsValid())
	{
		const EOnlineAsyncTaskState::Type searchstatus = SessionSearch->SearchState;
		if (searchstatus == EOnlineAsyncTaskState::InProgress)
		{
			retval = true;
		}
	}

	return retval;
}

void ULobbyGameInstance::InitDefaultSessionSettings(FOnlineSessionSettings& OutSettings) const
{
	OutSettings.NumPublicConnections = 4;
	OutSettings.bShouldAdvertise = true;
	OutSettings.bUsesPresence = true;
	OutSettings.bAllowInvites = true;
	OutSettings.bAllowJoinInProgress = true;
	OutSettings.bShouldAdvertise = true;
	OutSettings.bAllowJoinViaPresence = true;
	OutSettings.bAllowJoinViaPresenceFriendsOnly = false;
	OutSettings.bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName().ToString() == "NULL";

	InitCustomSessionSettings(OutSettings.Settings);
}

void ULobbyGameInstance::InitCustomSessionSettings(FSessionSettings& OutSettings) const
{

}

FOnlineSessionSettings ULobbyGameInstance::GetHostSettings(FSessionSettings InAdditionalSettings) const
{
	FOnlineSessionSettings retval = DefaultSessionSettings;
	retval.Settings.Append(InAdditionalSettings);

	return retval;
}

void ULobbyGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
	// It will not be success if there are more than one session with the same name already created
	if (!Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("[UCoopPuzzleGameInstance::OnCreateSessionComplete] UNSUCESS"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("[UNetTileMazeGameInstance::OnCreateSessionComplete] SUCESS SessionName: %s"), *SessionName.ToString());

	CloseLoadingScreen();

	UEngine* Engine = GetEngine();

	if (Engine == nullptr) return;

	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("[OnCreateSessionComplete::Host]"));

	UE_LOG(LogTemp, Warning, TEXT("[OnCreateSessionComplete::OnCreateSessionComplete] HOST TRAVEL TO LOBBY"));

	TravelToLobby();
}

void ULobbyGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	if (Success)
	{	
		if (RestartSession)
		{
			FStoredSessionSettings storedsettings;
			if (GetStoredSessionSettings(storedsettings))
			{
				CreateSession(storedsettings.Settings, storedsettings.ServerName);
				RestartSession = false;
				ClearStoredSettings();
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("[ULobbyGameInstance::OnDestroySessionComplete] Sesstion Restart Failed, Valid Session Settings not found"));
			}

		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[ULobbyGameInstance::OnDestroySessionComplete] NO Success "));
	}

	UWorld* World = GetWorld();
	if (World != nullptr)
	{
		UGameplayStatics::OpenLevel(World, FName(*MainMenuMapName));
	}
}

void ULobbyGameInstance::OnFindSessionsComplete(bool Success)
{
	FSessionSearchResults broadcast;

	if (Success && SessionSearch.IsValid())
	{
		const TArray<FOnlineSessionSearchResult> searchresults = SessionSearch->SearchResults;
		broadcast = FSessionSearchResults(searchresults);
	}
	else
	{
		broadcast = FSessionSearchResults();
		UE_LOG(LogTemp, Warning, TEXT("[ULobbyGameInstance::OnFindSessionsComplete] Error Session Search Failed"));
	}

	SearchResultsReadyDelegate.ExecuteIfBound(broadcast);
	SessionSearch.Reset();
}

void ULobbyGameInstance::OnJoinSessionsComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	CloseLoadingScreen();
	if (!SessionInterface.IsValid()) return;

	FString Url;
	if (!SessionInterface->GetResolvedConnectString(SESSION_NAME, Url))
	{
		UE_LOG(LogTemp, Warning, TEXT("[ULobbyGameInstance::OnJoinSessionsComplete] Couldn't get Connect String"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("[ULobbyGameInstance::OnJoinSessionsComplete] Url: %s"), *Url);

	APlayerController* PlayerController = GetFirstLocalPlayerController();

	if (PlayerController == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ULobbyGameGameInstance::OnJoinSessionsComplete] Player Controller DOESN'T EXIST"));
		return;
	}

	bIsPlayingOffline = false;
	PlayerController->ClientTravel(Url, ETravelType::TRAVEL_Absolute);

}

void ULobbyGameInstance::ReadFriendsList(FName SubSystemName)
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	IOnlineSubsystem * const OnlineSub = IOnlineSubsystem::Get(SubSystemName);

	if (PlayerController && OnlineSub)
	{
		IOnlineFriendsPtr FriendInterface = OnlineSub->GetFriendsInterface();

		if (FriendInterface.IsValid())
		{
			ULocalPlayer * LocalPlayer = PlayerController->GetLocalPlayer();

			if (LocalPlayer)
			{
				/*Store the System we used to read the FriendsList and query the interface*/
				LastReadSubSystem = SubSystemName;
				FriendInterface->ReadFriendsList(LocalPlayer->GetControllerId(), EFriendsLists::ToString(EFriendsLists::OnlinePlayers), ReadFriendListCompleteDelagate);
			}
		}
	}
}

void ULobbyGameInstance::OnReadFriendsListCompleted(int32 LocalUserNum, bool bWasSuccessful, const FString & ListName, const FString & ErrorString)
{
	if (bWasSuccessful)
	{
		//get the steam online subsystem from the last read
		IOnlineSubsystem* const OnlineSub = IOnlineSubsystem::Get(LastReadSubSystem);

		//check if the online subsystem is valid
		if (OnlineSub)
		{
			IOnlineFriendsPtr FriendsInterface = OnlineSub->GetFriendsInterface();

			//if the Friends Interface is valid
			if (FriendsInterface.IsValid())
			{
				TArray< TSharedRef<FOnlineFriend> > FriendList;
				//get a list on all online players and store them in the FriendList
				FriendsInterface->GetFriendsList(LocalUserNum, ListName, FriendList);

				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Emerald, *FString::Printf(TEXT("Number of friends found is: %d"), FriendList.Num()));
				
				TArray<FSubSytemFriendInfo> FriendsList;
				
				//for each loop to convert the FOnlineFriend array into the cuteom BP struct
				for (TSharedRef<FOnlineFriend> Friend : FriendList)
				{
					//temp FSteamFriendInfo variable to add to the array
					FSubSytemFriendInfo TempSteamFriendInfo;
					
					//get the friend's User ID
					TempSteamFriendInfo.PlayerUniqueNetID.SetUniqueNetId(Friend->GetUserId());
					
					//get the friend's avatar as texture 2D and store it
					TempSteamFriendInfo.PlayerAvatar = GetFriendAvatar(TempSteamFriendInfo.PlayerUniqueNetID, LastReadSubSystem);
					
					//get the friend's display name
					TempSteamFriendInfo.PlayerName = Friend->GetDisplayName();

					//add the temp variable to the 
					FriendsList.Add(TempSteamFriendInfo);
				}
				
				/*Fire the Delegate For Recieving UI*/
				FriendsListReadyDelegate.ExecuteIfBound(FriendsList);
			}
		}
	}
	else
	{
		//ShowErrorMessageUMG(FText::FromString(ErrorString));
	}

}

void ULobbyGameInstance::SendSessionInviteToFriend(APlayerController * InvitingPlayer, const FBPUniqueNetId & Friend)
{
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(InvitingPlayer->GetLocalPlayer());
		if (LocalPlayer)
		{
			// Get SessionInterface from the OnlineSubsystem
			IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
			if (Sessions.IsValid() && LocalPlayer->GetPreferredUniqueNetId().IsValid() && Friend.IsValid())
			{
				Sessions->SendSessionInviteToFriend(*LocalPlayer->GetPreferredUniqueNetId(), SESSION_NAME, *Friend.GetUniqueNetId());
			}
		}
	}
}

void ULobbyGameInstance::OnSessionUserInviteAccepted(bool bWasSuccessful, int32 LocalUserNum, TSharedPtr<const FUniqueNetId> InvitingPlayer, const FOnlineSessionSearchResult & TheSessionInvitedTo)
{
	// Get OnlineSubsystem we want to work with
	IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
	if (OnlineSub)
	{
		// Get SessionInterface from the OnlineSubsystem
		IOnlineSessionPtr Sessions = OnlineSub->GetSessionInterface();
		if (Sessions.IsValid())
		{
			Sessions->JoinSession(LocalUserNum, SESSION_NAME, TheSessionInvitedTo);
		}
	}
}

UTexture2D * ULobbyGameInstance::GetFriendAvatar(FBPUniqueNetId PlayerNetID, FName SubSystemName)
{
	return nullptr;
}

void ULobbyGameInstance::CreateSession(const FOnlineSessionSettings InSettings, const FString InServerName)
{
	if (SessionInterface.IsValid())
	{
		SessionInterface->CreateSession(0, SESSION_NAME, InSettings);
	}
}

void ULobbyGameInstance::StoreSessionSettings(FStoredSessionSettings InSettings)
{
	StoredSessionSettings = InSettings;
	InSettings.bIsValid = true;
}

void ULobbyGameInstance::ClearStoredSettings()
{
	StoredSessionSettings.bIsValid = false;
}

bool ULobbyGameInstance::GetStoredSessionSettings(FStoredSessionSettings& OutSettings) const
{
	OutSettings = StoredSessionSettings;

	return OutSettings.bIsValid;
}

bool ULobbyGameInstance::AreSessionSettingsStored() const
{
	return StoredSessionSettings.bIsValid;
}

void ULobbyGameInstance::EndSession()
{
	if (SessionInterface.IsValid() && (SessionInterface->GetNamedSession(SESSION_NAME) != nullptr))
	{
		/*Should be false but just to be sure*/
		RestartSession = false;
		SessionInterface->DestroySession(SESSION_NAME);
	}
	else if (bIsPlayingOffline)
	{
		UWorld* World = GetWorld();
		if (World != nullptr)
		{
			UGameplayStatics::OpenLevel(World, FName(*MainMenuMapName));
		}
	}
}