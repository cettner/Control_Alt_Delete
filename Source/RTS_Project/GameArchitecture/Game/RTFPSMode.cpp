// Fill out your copyright notice in the Description page of Project Settings.

#include "RTFPSMode.h"
#include "Engine.h"
#include "RTFPSGameState.h"
#include "RTFPSPlayerState.h"
#include "RTS_Project/RTSFPS/GameObjects/RTFPSPlayerStart.h"
#include "RTS_Project/RTSFPS/FPS/FPSServerController.h"
#include "RTS_Project/RTSFPS/RTS/Camera/RTSSelectionCamera.h"
#include "RTS_Project/RTSFPS/FPS/Commander.h"
#include "RTS_Project/RTSFPS/PreGame/RTSFPSLobbyGameState.h"


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
	if (InController && HasMatchStarted())
	{
		ARTFPSPlayerState * PlayerState = Cast<ARTFPSPlayerState>(InController->PlayerState);

		if (PlayerState == nullptr) return (nullptr);
		
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

AActor * ARTFPSMode::FindPlayerStart_Implementation(AController * Player, const FString & IncomingName)
{
	UWorld * World = GetWorld();
	ARTFPSPlayerState * PS = Cast<ARTFPSPlayerState>(Player->PlayerState);
	ADefaultGameState * GS = GetGameState<ADefaultGameState>();

	for (TActorIterator<ARTFPSPlayerStart> It(World); It; ++It)
	{
		ARTFPSPlayerStart * Start = *It;
		if (GS->IsTeamValid(Start->teamid) && Start->isRTSStart == PS->isRtsPlayer && Start->teamid == PS->TeamID)
		{
			return(Start);
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
		
		if (settings.CustomSettings.Contains(RTS_Project::IsRTSPlayerkey))
		{
			PS->isRtsPlayer = (bool)(*settings.CustomSettings.Find(RTS_Project::IsRTSPlayerkey));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("[ARTSFPSMODE::FinishPlayerRegistration] Failed to Load Custom Setting"));
		}
	}

	return RetParent;
}
