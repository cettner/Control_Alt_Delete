// Fill out your copyright notice in the Description page of Project Settings.

#include "RTFPSPlayerState.h"
#include "RTFPSGameState.h"
#include "Engine/World.h"

void ARTFPSPlayerState::BeginPlay()
{
	Super::BeginPlay();

	UWorld * World = GetWorld();
	if (HasAuthority() && World && World->GetGameState<ARTFPSGameState>())
	{
		ARTFPSGameState * GS = World->GetGameState<ARTFPSGameState>();

		if (!GS->NumRTSPlayers(Team_ID))
		{
			isRtsPlayer = true;
		}
	}

}
