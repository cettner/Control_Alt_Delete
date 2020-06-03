// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameInstance.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystemTypes.h"

const static FName SESSION_NAME = TEXT("CoopPuzzleGameSession");
const static FName SERVER_NAME_SETTINGS_KEY = TEXT("ServerName");

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
		UE_LOG(LogTemp, Warning, TEXT("[ULobbyGameInstance::OpenSessionListMenu] Session is valid"));
		//SessionSearch->bIsLanQuery = true;
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

void ULobbyGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
}

void ULobbyGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
}

void ULobbyGameInstance::OnFindSessionsComplete(bool Success)
{
}

void ULobbyGameInstance::OnJoinSessionsComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
}

void ULobbyGameInstance::CreateSession()
{
}


void ULobbyGameInstance::EndSession()
{

}