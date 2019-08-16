// Fill out your copyright notice in the Description page of Project Settings.

#include "RTFPSMode.h"
#include "GameAssets.h"
#include "Engine.h"
#include "RTFPSGameState.h"
#include "RTFPSPlayerState.h"
#include "RTFPSPlayerStart.h"
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

	RTSStartingPoints = TeamStartingPoints;
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

AActor * ARTFPSMode::FindPlayerStart_Implementation(AController * Player, const FString & IncomingName)
{
	if (Cast<APlayerStart>(Player->StartSpot))
	{
		return(Cast<APlayerStart>(Player->StartSpot));
	}

	UWorld* World = GetWorld();
	ARTFPSPlayerState * PS = Cast<ARTFPSPlayerState>(Player->PlayerState);
	ADefaultGameState * GS = GetGameState<ADefaultGameState>();

	if (PS && GS)
	{
		if (PS->isRtsPlayer && GS->IsTeamValid(PS->Team_ID) && TeamStartingPoints.Num())
		{
			return(RTSStartingPoints[PS->Team_ID].GetNextSpawn());
		}
		else if (GS->IsTeamValid(PS->Team_ID) && TeamStartingPoints.Num())
		{
			return(TeamStartingPoints[PS->Team_ID].GetNextSpawn());
		}
	}

	return (ChoosePlayerStart(Player));
}

void ARTFPSMode::BeginPlay()
{
	Super::BeginPlay();
	UWorld * World = GetWorld();
	ADefaultGameState * GS = GetGameState<ADefaultGameState>();

	for (int i = 0; i < TeamStartingPoints.Num(); i++)
	{
		ARTFPSPlayerStart * Start = Cast<ARTFPSPlayerStart>(TeamStartingPoints[i].GetNextSpawn());
		if(Start && Start->isRTSStart && GS->IsTeamValid(Start->teamid))
		{
			RTSStartingPoints[Start->teamid].Add(Start);
			TeamStartingPoints.RemoveAt(i);
			i--;
		}
	}
			
}
