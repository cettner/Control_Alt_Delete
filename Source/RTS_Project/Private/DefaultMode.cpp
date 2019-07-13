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
	DefaultFPSClass = ACommander::StaticClass();
	DefaultRTSClass = ARTSCamera::StaticClass();
}

void ADefaultMode::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);
	ARTSPlayerController * PC =  Cast<ARTSPlayerController>(NewPlayer);
	ADefaultGameState * GS =  GetGameState<ADefaultGameState>();

	if (PC && GS)
	{
		PC->FinishPlayerLogin();
	}
}

void ADefaultMode::Logout(AController * Exiting)
{
	Super::Logout(Exiting);
}


UClass * ADefaultMode::GetDefaultPawnClassForController_Implementation(AController * InController)
{
	if (Cast<ARTSPlayerController>(InController))
	{
		return(nullptr);
	}
	else
	{
		return(Super::GetDefaultPawnClassForController_Implementation(InController));
	}
}

