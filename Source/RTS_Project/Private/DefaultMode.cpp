// Fill out your copyright notice in the Description page of Project Settings.

#include "DefaultMode.h"
#include "DefaultGameState.h"
#include "RTSPlayerController.h"
#include "GameAssets.h"
#include "Engine.h"
#include "CombatCommander.h"

ADefaultMode::ADefaultMode(const FObjectInitializer& ObjectInitializer) 
: Super(ObjectInitializer)
{
}

void ADefaultMode::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);
	ARTSPlayerController * PC =  Cast<ARTSPlayerController>(NewPlayer);
	if (PC)
	{
		PC->FinishPlayerLogin();
	}

}

void ADefaultMode::Logout(AController * Exiting)
{
	Super::Logout(Exiting);
}
