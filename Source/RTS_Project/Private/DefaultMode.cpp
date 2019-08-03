// Fill out your copyright notice in the Description page of Project Settings.

#include "DefaultMode.h"
#include "DefaultGameState.h"
#include "FPSServerController.h"
#include "GameAssets.h"
#include "Engine.h"
#include "RTSSelectionCamera.h"
#include "CombatCommander.h"

ADefaultMode::ADefaultMode(const FObjectInitializer& ObjectInitializer) 
: Super(ObjectInitializer)
{
	ConstructorHelpers::FObjectFinder<UBlueprint> TargetBlueprint(TEXT(COMBAT_COMMANDER_BP_PATH));
	if (TargetBlueprint.Object)
	{
		DefaultFPSClass = (UClass*)TargetBlueprint.Object->GeneratedClass;
	}

	DefaultRTSClass = ARTSSelectionCamera::StaticClass();
	GameStateClass = ADefaultGameState::StaticClass();
	PlayerControllerClass = AFPSServerController::StaticClass();
	HUDClass = ARTSHUD::StaticClass();
	PlayerStateClass = ADefaultPlayerState::StaticClass();
	DefaultPawnClass = nullptr;
}

void ADefaultMode::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);
	ARTSPlayerController * PC =  Cast<ARTSPlayerController>(NewPlayer);
	ADefaultGameState * GS =  GetGameState<ADefaultGameState>();
	ADefaultPlayerState * PS = Cast<ADefaultPlayerState>(NewPlayer->PlayerState);

	
	if (PC && GS && PS)
	{
		PS->Team_ID = GS->AssignBalancedTeam(PS);
		PC->FinishPlayerLogin();
	}
}

void ADefaultMode::Logout(AController * Exiting)
{
	Super::Logout(Exiting);
}


UClass * ADefaultMode::GetDefaultPawnClassForController_Implementation(AController * InController)
{
	ARTSPlayerController * PC = Cast<ARTSPlayerController>(InController);
	if (PC)
	{
		ADefaultPlayerState * PlayerState = Cast<ADefaultPlayerState>(PC->PlayerState);
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

