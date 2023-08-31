// Fill out your copyright notice in the Description page of Project Settings.

#include "RTFPSPlayerState.h"
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

void ARTFPSPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(ARTFPSPlayerState, isRtsPlayer, COND_InitialOnly);
}
