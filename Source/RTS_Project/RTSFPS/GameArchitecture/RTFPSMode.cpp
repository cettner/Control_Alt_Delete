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
#include "RTS_Project/RTSFPS/Shared/Interfaces/RTSObjectInterface.h"
#include "TeamResourceState.h"
#include "RTS_Project/RTSFPS/Gamesystems/ResourceSystem/MineableResource.h"


ARTFPSMode::ARTFPSMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
	DefaultFPSClass = ACommander::StaticClass();
	DefaultRTSClass = ARTSSelectionCamera::StaticClass();
	GameStateClass = ARTFPSGameState::StaticClass();
	PlayerControllerClass = AFPSServerController::StaticClass();
	HUDClass = ARTSHUD::StaticClass();
	PlayerStateClass = AFPSPlayerState::StaticClass();
	TeamStateClass = ATeamResourceState::StaticClass();
	DefaultPawnClass = nullptr;
	ResourceTypes = TArray<TSubclassOf<UResource>>();
	StartingResource = FReplicationResourceMap();

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
	UWorld * world = GetWorld();
	const ARTFPSPlayerState * PS = Cast<ARTFPSPlayerState>(Player->PlayerState);
	AActor * retval = nullptr;

	if (!HasMatchStarted())
	{
		retval = ChoosePlayerStart(Player);
	}
	else
	{
		for (TActorIterator<ATeamPlayerStart> It(world); It; ++It)
		{
			ATeamPlayerStart * start = *It;
			if (start->teamid == PS->GetTeamID())
			{
				retval = start;
				ARTFPSPlayerStart * rtsfpsstart = Cast<ARTFPSPlayerStart>(start);

				if (rtsfpsstart->isRTSStart == PS->IsRTSPlayer())
				{
					retval = rtsfpsstart;
					break;
				}
			}
		}
	}


	return (retval);
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

bool ARTFPSMode::ReadyToEndMatch_Implementation()
{

	bool retval = true;

	for (TActorIterator<AMineableResource> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		const AMineableResource * mapresource = *ActorItr;

		if (IsValid(mapresource))
		{
			retval = false;
			break;
		}
	}

	if (retval == true)
	{
		for (TActorIterator<ARTSMinion> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			const ARTSMinion * mapminion = *ActorItr;
			if (IsValid(mapminion))
			{
				const IResourceGatherer* gatherer = Cast<IResourceGatherer>(mapminion);

				if (gatherer != nullptr  && gatherer->GetCurrentWeight() > 0U)
				{
					retval = false;
					break;
				}
			}

		}
	}


	return retval;
}

TSubclassOf<ARTSCamera> ARTFPSMode::GetDefaultRTSClass() const
{
	return DefaultRTSClass;
}

FReplicationResourceMap ARTFPSMode::GetStartingResources() const
{
	return StartingResource;
}

TArray<TSubclassOf<UResource>> ARTFPSMode::GetResourceTypes() const
{
	return ResourceTypes;
}

TMap<TSubclassOf<UObject>, FReplicationResourceMap> ARTFPSMode::GetDefaultUnitCosts() const
{
	return DefaultUnitCosts;
}

TSubclassOf<ACommander> ARTFPSMode::GetDefaultFPSClass() const
{
	return DefaultFPSClass;
}

UCurveFloat* ARTFPSMode::GetExpCurve() const
{
	return ExpCurve;
}

uint32 ARTFPSMode::GetMaxLevel() const
{
	return MaxLevel;
}
