// Fill out your copyright notice in the Description page of Project Settings.

#include "RTFPSMode.h"
#include "Engine.h"
#include "RTFPSGameState.h"
#include "RTFPSPlayerState.h"
#include "RTSFPS/GameObjects/RTFPSPlayerStart.h"
#include "RTSFPS/FPS/FPSServerController.h"
#include "RTSFPS/RTS/Camera/RTSSelectionCamera.h"
#include "RTSFPS/FPS/Commander.h"
#include "RTSFPS/PreGame/RTSFPSLobbyGameState.h"

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
		if (PS->isRtsPlayer && GS->IsTeamValid(PS->TeamID) && TeamStartingPoints[PS->TeamID].Num())
		{
			return(TeamStartingPoints[PS->TeamID].GetNextSpawn());
		}
		else if (GS->IsTeamValid(PS->TeamID) && TeamStartingPoints[PS->TeamID].Num())
		{
			return(TeamStartingPoints[PS->TeamID].GetNextSpawn());
		}
	}

	return (ChoosePlayerStart(Player));
}

bool ARTFPSMode::FinishPlayerRegistration(ADefaultPlayerController* RegisteringPlayer, FPlayerSettings settings)
{
	bool RetParent = Super::FinishPlayerRegistration(RegisteringPlayer, settings);
	if (RetParent)
	{
		ARTFPSPlayerState* PS = RegisteringPlayer->GetPlayerState<ARTFPSPlayerState>();
		if (PS == nullptr) return false;
		
		if (settings.CustomSettings.Find(RTS_Project::IsRTSPlayerkey))
		{
			PS->isRtsPlayer = StaticCast<bool>(settings.CustomSettings.Find(RTS_Project::IsRTSPlayerkey));
		}
	}

	return RetParent;
}
