// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameSession.h"
#include "LobbyGameSession.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API ALobbyGameSession : public AGameSession
{
	GENERATED_BODY()

	virtual void RegisterPlayer(APlayerController* NewPlayer, const TSharedPtr<const FUniqueNetId>& UniqueId, bool bWasFromInvite) override;
	
};
