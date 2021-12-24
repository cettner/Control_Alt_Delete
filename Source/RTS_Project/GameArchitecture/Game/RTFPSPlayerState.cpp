// Fill out your copyright notice in the Description page of Project Settings.

#include "RTFPSPlayerState.h"
#include "RTS_Project/RTSFPS/BaseClasses/RTSPlayerController.h"
#include "RTFPSGameState.h"
#include "Engine/World.h"
#include "Net/UnrealNetwork.h"


bool ARTFPSPlayerState::IsRTSPlayer() const
{
	return(isRtsPlayer);
}

void ARTFPSPlayerState::SetIsRTSPlayer(bool isrts)
{
	isRtsPlayer = isrts;
}

void ARTFPSPlayerState::SetTeamStructures(TArray<ARTSStructure*>& InStructures)
{
	TeamStructures = InStructures;
}

TArray<ARTSStructure *> ARTFPSPlayerState::GetTeamStructures() const
{
	return TeamStructures;
}

void ARTFPSPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(ARTFPSPlayerState, isRtsPlayer, COND_InitialOrOwner);
	DOREPLIFETIME(ARTFPSPlayerState,TeamStructures)
}
