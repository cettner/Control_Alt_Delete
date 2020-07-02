// Fill out your copyright notice in the Description page of Project Settings.

#include "RTFPSPlayerState.h"
#include "RTSFPS/BaseClasses/RTSPlayerController.h"
#include "RTFPSGameState.h"
#include "Engine/World.h"
#include "UnrealNetwork.h"

#define FPSONLY

void ARTFPSPlayerState::BeginPlay()
{
	Super::BeginPlay();
}

void ARTFPSPlayerState::ClientInitialize(AController * C)
{
	Super::ClientInitialize(C);
	UWorld* World = GetWorld();
	if (World == nullptr) return;
	
	ARTSPlayerController* PC = Cast<ARTSPlayerController>(C);
	if (PC == nullptr) return;

	PC->FinishLocalPlayerSetup(this);
}

void ARTFPSPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(ARTFPSPlayerState,isRtsPlayer, COND_InitialOrOwner)
}
