// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "LobbyPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API ALobbyPlayerState : public APlayerState
{
	GENERATED_BODY()

	protected:
	void ClientInitialize(AController* Controller) override;
};
