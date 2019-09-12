// Fill out your copyright notice in the Description page of Project Settings.

#include "RTFPSPlayerState.h"
#include "RTSPlayerController.h"
#include "RTFPSGameState.h"
#include "Engine/World.h"
#include "UnrealNetwork.h"

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
	else if (!HasAuthority() && World && World->GetFirstPlayerController())
	{
		ARTSPlayerController * PC= Cast<ARTSPlayerController>(World->GetFirstPlayerController());
		if (PC)
		{
			PC->FinishLocalPlayerSetup(this);
		}
	}

}

void ARTFPSPlayerState::ClientInitialize(AController * C)
{
	Super::ClientInitialize(C);
}

void ARTFPSPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(ARTFPSPlayerState,isRtsPlayer, COND_InitialOrOwner)
}
