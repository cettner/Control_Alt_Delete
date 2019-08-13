// Fill out your copyright notice in the Description page of Project Settings.

#include "RTFPSMode.h"
#include "GameAssets.h"
#include "Engine.h"
#include "RTFPSGameState.h"
#include "RTFPSPlayerState.h"
#include "FPSServerController.h"
#include "RTSSelectionCamera.h"
#include "CombatCommander.h"

ARTFPSMode::ARTFPSMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
	ConstructorHelpers::FObjectFinder<UBlueprint> TargetBlueprint(TEXT(COMBAT_COMMANDER_BP_PATH));
	if (TargetBlueprint.Object)
	{
		DefaultFPSClass = (UClass*)TargetBlueprint.Object->GeneratedClass;
	}

	DefaultRTSClass = ARTSSelectionCamera::StaticClass();
	GameStateClass = ARTFPSGameState::StaticClass();
	PlayerControllerClass = AFPSServerController::StaticClass();
	HUDClass = ARTSHUD::StaticClass();
	PlayerStateClass = ARTFPSPlayerState::StaticClass();
	DefaultPawnClass = nullptr;
}

UClass * ARTFPSMode::GetDefaultPawnClassForController_Implementation(AController * InController)
{
	ADefaultGameState * GS = GetGameState<ADefaultGameState>();
	if (InController && GS)
	{
		ARTFPSPlayerState * PlayerState = Cast<ARTFPSPlayerState>(InController->PlayerState);

		if (PlayerState)
		{
			if (PlayerState->isRtsPlayer)
			{
				return(DefaultRTSClass);
			}
			else
			{
				return(DefaultFPSClass);
			}
		}
		else
		{
			return(nullptr);
		}

	}
	else
	{
		return(nullptr);
	}
}
