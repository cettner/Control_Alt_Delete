// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../GameArchitecture/Lobby/LobbyGameState.h"
#include "RTSFPSLobbyGameState.generated.h"

/**
 * 
 */

namespace RTS_Project
{
	const FString IsRTSPlayerkey = "bIsRTSPlayer";
}


UCLASS()
class RTS_PROJECT_API ARTSFPSLobbyGameState : public ALobbyGameState
{
	GENERATED_BODY()

	protected:
	virtual bool IsRTSPlayerSlot(const FSlotPlayerData StoredSlot);

	protected:
	virtual void SetCustomPlayerSettings(FPlayerSettings& outsettings, const FSlotPlayerData StoredSlot) override;
};
