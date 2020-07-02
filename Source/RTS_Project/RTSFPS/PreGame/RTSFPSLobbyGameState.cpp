// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSFPSLobbyGameState.h"




bool ARTSFPSLobbyGameState::IsRTSPlayerSlot(const FSlotPlayerData StoredSlot)
{
	return (StoredSlot.SlotId == 0);
}

void ARTSFPSLobbyGameState::SetCustomPlayerSettings(FPlayerSettings& outsettings, const FSlotPlayerData StoredSlot)
{
	Super::SetCustomPlayerSettings(outsettings, StoredSlot);

	if (IsRTSPlayerSlot(StoredSlot))
	{
		outsettings.CustomSettings.Emplace(RTS_Project::IsRTSPlayerkey, true);
	}
	else
	{
		outsettings.CustomSettings.Emplace(RTS_Project::IsRTSPlayerkey, false);
	}
}
