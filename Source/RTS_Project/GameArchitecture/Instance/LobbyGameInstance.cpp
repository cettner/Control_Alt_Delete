 // Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameInstance.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystemTypes.h"
#include "PreGame/MainMenu/MainMenu.h"
#include "Kismet/GameplayStatics.h"
#include "PreGame/LobbySystem/LobbyMenu.h"

const static FName SESSION_NAME = TEXT("RTSFPSGameSession");
const static FName SERVER_NAME_SETTINGS_KEY = TEXT("ServerName");


ULobbyGameInstance::ULobbyGameInstance(const FObjectInitializer& ObjectInitializer)
{
	MenuClass = UMainMenu::StaticClass();
	LobbyClass = UMainMenu::StaticClass();
	RestartSession = false;
	DesiredServerName = "Default Server";
	PlayerName = "";
	LobbyMapName = "/Game/Maps/LobbyMap";
	GameMapName = "/Game/Maps/DevMap";
	MainMenuMapName = "/Game/Maps/MainMenuMap";
}

void ULobbyGameInstance::Init()
{
	IOnlineSubsystem* SubSystem = IOnlineSubsystem::Get();

	if (SubSystem != nullptr)
	{
		SessionInterface = SubSystem->GetSessionInterface();

		if (SessionInterface.IsValid())
		{
			UE_LOG(LogTemp, Warning, TEXT("[ULobbyGameInstance::Init] SessionInterface.IsValid"));
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &ULobbyGameInstance::OnCreateSessionComplete);

			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &ULobbyGameInstance::OnDestroySessionComplete);

			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &ULobbyGameInstance::OnFindSessionsComplete);

			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &ULobbyGameInstance::OnJoinSessionsComplete);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[ULobbyGameInstance::Init] Found NO SUBSYSTEM"));
	}
}

void ULobbyGameInstance::LoadMainMenu()
{
	if (MenuClass == nullptr)
	{
		return;
	}
	else if (MainMenu == nullptr)
	{
		MainMenu = CreateWidget<UMainMenu>(this, MenuClass);
		if (MainMenu == nullptr) return;
	}
	
	MainMenu->Setup(this);
	
}

void ULobbyGameInstance::LoadLobbyMenu()
{
	if (LobbyClass == nullptr)
	{
		return;
	}
	else if (LobbyMenu == nullptr)
	{
		LobbyMenu = CreateWidget<ULobbyMenu>(this, LobbyClass);
		if (LobbyMenu == nullptr) return;
	}
	
	LobbyMenu->Setup();
}

void ULobbyGameInstance::StartGame()
{
	UWorld* World = GetWorld();
	if (World == nullptr) return;

	FString modifier = "?listen";
	World->ServerTravel(GameMapName + modifier);
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

FPlayerSettings ULobbyGameInstance::GetPlayerSettings()
{
	return PlayerSettings;
}

bool ULobbyGameInstance::SetServerSettings(FServerSettings settings)
{
	ServerSettings = settings;
	return true;
}

FServerSettings ULobbyGameInstance::GetServerSettings()
{
	return(ServerSettings);
}

void ULobbyGameInstance::Host(FString ServerName)
{
	DesiredServerName = ServerName;

	if (SessionInterface.IsValid())
	{
		// Checks for an existing session
		auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);

		if (ExistingSession != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("[ULobbyGameInstance::Host] There is an existing session about to remove the current one"));
			RestartSession = true;
			SessionInterface->DestroySession(SESSION_NAME);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("[ULobbyGameInstance::Host] About to create session"));

			// Create a new session
			CreateSession();

		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[ULobbyGameInstance::Host] SessionInterface invalid"));
	}
}

void ULobbyGameInstance::JoinSession(uint32 Index)
{
	if (!SessionInterface.IsValid() || (!SessionSearch.IsValid())) return;

	if (Index < (uint32)(SessionSearch->SearchResults.Num()))
	{
		SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);
	}
}

void ULobbyGameInstance::OpenSessionListMenu()
{
//	if (MainMenu == nullptr) return;

	SessionSearch = MakeShareable(new FOnlineSessionSearch());

	if (SessionSearch.IsValid())
	{
		SessionSearch->bIsLanQuery = true;
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[ULobbyGameInstance::OpenSessionListMenu] Session is Invalid"));
	}
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

	// Teardown Menu and change levels
	if (MainMenu != nullptr)
	{
		MainMenu->Teardown();
		MainMenu = nullptr;
	}

	UEngine* Engine = GetEngine();

	if (Engine == nullptr) return;

	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("[OnCreateSessionComplete::Host]"));

	UE_LOG(LogTemp, Warning, TEXT("[OnCreateSessionComplete::OnCreateSessionComplete] HOST TRAVEL TO LOBBY"));

	UWorld* World = GetWorld();

	if (World == nullptr) return;

	/*StartMap as Listen Server*/
	FString modifier = "?listen";

	World->ServerTravel(LobbyMapName + modifier);
}

void ULobbyGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	if (Success)
	{	
		if (RestartSession)
		{
			CreateSession();
			RestartSession = false;
		}
		else
		{
			UWorld* World = GetWorld();
			if (World != nullptr)
			{
				UGameplayStatics::OpenLevel(World, FName(*MainMenuMapName));
			}

		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[ULobbyGameInstance::OnDestroySessionComplete] NO Success "));
	}
}

void ULobbyGameInstance::OnFindSessionsComplete(bool Success)
{
	if (MainMenu == nullptr) return;

	if (Success && SessionSearch.IsValid())
	{
		if (SessionSearch->SearchResults.Num() <= 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("[ULobbyGameInstance::OnFindSessionsComplete] No Sessions Found"));
		}
		else
		{
			TArray<FServerData> ServerData;
			for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
			{
				UE_LOG(LogTemp, Warning, TEXT("[ULobbyGameInstance::OnFindSessionsComplete] Session Name %s"), *SearchResult.GetSessionIdStr());

				FServerData Data;
				FString ServerName;
				if (SearchResult.Session.SessionSettings.Get(SERVER_NAME_SETTINGS_KEY, ServerName))
				{
					UE_LOG(LogTemp, Warning, TEXT("[ULobbyGameInstance::OnFindSessionsComplete] Data found in settings %s"), *ServerName);
					Data.Name = ServerName;
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("[ULobbyGameInstance::OnFindSessionsComplete] Data NOT found in settings"));

					Data.Name = "Could not find name";
				}

				Data.MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
				Data.CurrentPlayers = Data.MaxPlayers - SearchResult.Session.NumOpenPublicConnections;
				Data.HostUsername = SearchResult.Session.OwningUserName;

				ServerData.Add(Data);
			}
			MainMenu->InitializeSessionsList(ServerData);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("[ULobbyGameInstance::OnFindSessionsComplete] Error session not found"));
	}
}

void ULobbyGameInstance::OnJoinSessionsComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (MainMenu != nullptr)
	{
		MainMenu->Teardown();
		MainMenu = nullptr;
	}
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

	PlayerController->ClientTravel(Url, ETravelType::TRAVEL_Absolute);

}

void ULobbyGameInstance::CreateSession()
{
	UE_LOG(LogTemp, Warning, TEXT("[ULobbyGameInstance::CreateSession] Creating %s"), *SESSION_NAME.ToString());

	if (SessionInterface.IsValid())
	{

		FOnlineSessionSettings SessionSettings;

		// Switch between bIsLANMatch when using NULL subsystem
		//if (IOnlineSubsystem::Get()->GetSubsystemName().ToString() == "NULL")
		//{
			SessionSettings.bIsLANMatch = true;
		//}
		//else
		//{
		//	SessionSettings.bIsLANMatch = false;
		//}

		if (DesiredServerName == "")
		{
			DesiredServerName = "Default Server";
		}


		// Number of sessions
		SessionSettings.NumPublicConnections = 2;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bUsesPresence = true;
		SessionSettings.bAllowInvites = true;
		SessionSettings.bAllowJoinInProgress = true;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bAllowJoinViaPresence = true;
		SessionSettings.bAllowJoinViaPresenceFriendsOnly = false;

		//Custom Game Settings
		SessionSettings.Set(SERVER_NAME_SETTINGS_KEY, DesiredServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
	}
}

void ULobbyGameInstance::EndSession()
{

	// Teardown Menu and change levels
	if (LobbyMenu != nullptr)
	{
		LobbyMenu->Teardown();
		LobbyMenu = nullptr;
	}

	if (SessionInterface.IsValid() && (SessionInterface->GetNamedSession(SESSION_NAME) != nullptr))
	{
		/*Should be false but just to be sure*/
		RestartSession = false;
		SessionInterface->DestroySession(SESSION_NAME);
	}
}