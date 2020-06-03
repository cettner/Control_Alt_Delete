// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionInterface.h"
#include "PreGame/LobbySystem/SessionMenuInterface.h"
#include "LobbyGameInstance.generated.h"



/**
 * 
 */
UCLASS()
class RTS_PROJECT_API ULobbyGameInstance : public UGameInstance, public ISessionMenuInterface
{
	GENERATED_BODY()
	
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
	FString DesiredServerName;
	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void OnFindSessionsComplete(bool Success);
	void OnJoinSessionsComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	void CreateSession();
private:

	// Session
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

};
