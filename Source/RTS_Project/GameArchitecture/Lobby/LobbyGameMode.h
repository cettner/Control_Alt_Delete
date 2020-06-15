// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API ALobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()

		virtual void PostLogin(APlayerController* NewPlayer) override;
	    virtual void Logout(AController * Exiting) override;
		ALobbyGameMode(const FObjectInitializer& ObjectInitializer);
	
};
