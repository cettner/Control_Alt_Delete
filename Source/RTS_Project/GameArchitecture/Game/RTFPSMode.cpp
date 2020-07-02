// Fill out your copyright notice in the Description page of Project Settings.

#include "RTFPSMode.h"
#include "Engine.h"
#include "RTFPSGameState.h"
#include "RTFPSPlayerState.h"
#include "RTSFPS/GameObjects/RTFPSPlayerStart.h"
#include "RTSFPS/FPS/FPSServerController.h"
#include "RTSFPS/RTS/Camera/RTSSelectionCamera.h"
#include "RTSFPS/FPS/Commander.h"

ARTFPSMode::ARTFPSMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
	DefaultFPSClass = ACommander::StaticClass();
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
	if (InController)
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
	UWorld* World = GetWorld();
	ARTFPSPlayerState * PS = Cast<ARTFPSPlayerState>(Player->PlayerState);
	ADefaultGameState * GS = GetGameState<ADefaultGameState>();

	if (PS && GS)
	{
		if (PS->isRtsPlayer && GS->IsTeamValid(PS->TeamID) && RTSStartingPoints[PS->TeamID].Num())
		{
			return(RTSStartingPoints[PS->TeamID].GetNextSpawn());
		}
		else if (GS->IsTeamValid(PS->TeamID) && TeamStartingPoints[PS->TeamID].Num())
		{
			return(TeamStartingPoints[PS->TeamID].GetNextSpawn());
		}
	}

	return (ChoosePlayerStart(Player));
}

APawn* ARTFPSMode::SpawnDefaultPawnFor_Implementation(AController* NewPlayer, AActor* StartSpot)
{
	APawn* startpawn = Super::SpawnDefaultPawnFor(NewPlayer, StartSpot);
	
	ACommander* FPSPawn = Cast<ACommander>(startpawn);
	ADefaultPlayerState* PS = NewPlayer->GetPlayerState<ADefaultPlayerState>();
	
	if (FPSPawn && PS)
	{
		FPSPawn->SetTeam(PS->TeamID);
	}
	
	return startpawn;
}

APawn* ARTFPSMode::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform)
{
	APawn* startpawn = Super::SpawnDefaultPawnAtTransform(NewPlayer, SpawnTransform);

	ACommander* FPSPawn = Cast<ACommander>(startpawn);
	ADefaultPlayerState* PS = NewPlayer->GetPlayerState<ADefaultPlayerState>();

	if (FPSPawn && PS)
	{
		FPSPawn->SetTeam(PS->TeamID);
	}

	return startpawn;
}

void ARTFPSMode::BeginPlay()
{
	Super::BeginPlay();
	ADefaultGameState * GS = GetGameState<ADefaultGameState>();

	//for (int i = 0; i < TeamStartingPoints.Num(); i++)
	//{
	//	for (int k = 0; k < TeamStartingPoints[i].Num(); k++)
	//	{
	//		ARTFPSPlayerStart * Start = Cast<ARTFPSPlayerStart>(TeamStartingPoints[i].GetNextSpawn());
	//		if (Start && Start->isRTSStart && GS->IsTeamValid(Start->teamid))
	//		{
	//			RTSStartingPoints[Start->teamid].Add(Start);
	//		}
	//	}
	//}
			
}
