// Fill out your copyright notice in the Description page of Project Settings.

#include "RTFPSMode.h"
#include "Engine.h"
#include "RTFPSGameState.h"
#include "RTFPSPlayerState.h"
#include "RTS_Project/RTSFPS/GameObjects/RTFPSPlayerStart.h"
#include "RTS_Project/RTSFPS/FPS/FPSServerController.h"
#include "RTS_Project/RTSFPS/FPS/FPSPlayerState.h"
#include "RTS_Project/RTSFPS/RTS/Camera/RTSSelectionCamera.h"
#include "RTS_Project/RTSFPS/FPS/Commander.h"
#include "RTS_Project/RTSFPS/PreGame/RTSFPSLobbyGameState.h"
#include "RTS_Project/RTSFPS/BaseClasses/Interfaces/RTSObjectInterface.h"


ARTFPSMode::ARTFPSMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
	DefaultFPSClass = ACommander::StaticClass();
	DefaultRTSClass = ARTSSelectionCamera::StaticClass();
	GameStateClass = ARTFPSGameState::StaticClass();
	PlayerControllerClass = AFPSServerController::StaticClass();
	HUDClass = ARTSHUD::StaticClass();
	PlayerStateClass = AFPSPlayerState::StaticClass();
	DefaultPawnClass = nullptr;
	ResourceTypes = TArray<TSubclassOf<AResource>>();

}

UClass * ARTFPSMode::GetDefaultPawnClassForController_Implementation(AController * InController)
{
	if (InController && HasMatchStarted())
	{
		ARTFPSPlayerState * PlayerState = Cast<ARTFPSPlayerState>(InController->PlayerState);

		if (PlayerState == nullptr) return (nullptr);
		
		if (PlayerState->IsRTSPlayer())
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
		if (GS->IsTeamValid(Start->teamid) && Start->isRTSStart == PS->IsRTSPlayer() && Start->teamid == PS->TeamID)
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
			const bool rtssetting = (bool)(*settings.CustomSettings.Find(RTS_Project::IsRTSPlayerkey));
			PS->SetIsRTSPlayer(rtssetting);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("[ARTSFPSMODE::FinishPlayerRegistration] Failed to Load Custom Setting"));
		}
	}

	return RetParent;
}

void ARTFPSMode::InitializeDeferredDefaultPawn(APawn * DefferedPawn, AController * InheritingController)
{
	if (GetNetMode() == NM_ListenServer)
	{
		ACommander * commandpawn = Cast<ACommander>(DefferedPawn);
		APlayerController * controller = GetWorld()->GetFirstPlayerController();

		if (commandpawn != nullptr && (InheritingController == controller))
		{
			commandpawn->SetIsServerPawn(true);
		}
	}

	IRTSObjectInterface * rtsobj = Cast<IRTSObjectInterface>(DefferedPawn);
	if (rtsobj != nullptr)
	{
		/*Add the Pawns to the GameState*/
		ARTFPSGameState * gs = GetGameState<ARTFPSGameState>();
		ADefaultPlayerController * pc = Cast<ADefaultPlayerController>(InheritingController);
		rtsobj->SetTeam(pc->GetTeamID());
		gs->AddRTSObjectToTeam(rtsobj);
	}

}

void ARTFPSMode::StartMatch()
{
	Super::StartMatch();
	ARTFPSGameState * GS = GetGameState<ARTFPSGameState>();

	GS->RefreshAllUnits();
}

int ARTFPSMode::GetStartingResources(TSubclassOf<AResource> ResourceClass)
{
	return StartingResource;
}

TArray<TSubclassOf<AResource>> ARTFPSMode::GetResourceTypes() const
{
	return ResourceTypes;
}

TMap<TSubclassOf<UObject>, FReplicationResourceMap> ARTFPSMode::GetDefaultUnitCosts() const
{
	return DefaultUnitCosts;
}
