// Fill out your copyright notice in the Description page of Project Settings.

#include "DefaultMode.h"
#include "DefaultGameState.h"
#include "RTSPlayerController.h"
#include "GameAssets.h"
#include "CombatCommander.h"

ADefaultMode::ADefaultMode(const FObjectInitializer& ObjectInitializer) 
: Super(ObjectInitializer)
{
	GameStateClass = ADefaultGameState::StaticClass();
	PlayerControllerClass = ARTSPlayerController::StaticClass();

	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT(COMBAT_COMMANDER_BP_PATH));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void ADefaultMode::PostLogin(APlayerController * NewPlayer)
{
	ARTSPlayerController * PC =  Cast<ARTSPlayerController>(NewPlayer);
	if (PC)
	{

	}
	else
	{

	}
}

void ADefaultMode::Logout(AController * Exiting)
{
}
