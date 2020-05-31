// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "LobbyGameInstance.generated.h"


/**
 * 
 */
UCLASS()
class RTS_PROJECT_API ULobbyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	


private:

	// Session
	IOnlineSessionPtr SessionInterface;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

};
