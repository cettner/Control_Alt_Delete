// Fill out your copyright notice in the Description page of Project Settings.

#include "RTSPlayerController.h"
#include "RTS_Project/RTSFPS/RTS/Structures/RTSStructure.h"
#include "RTS_Project/RTSFPS/GameArchitecture/RTFPSPlayerState.h"
#include "RTS_Project/RTSFPS/FPS/Commander.h"
#include "RTS_Project/RTSFPS/GameArchitecture/RTFPSGameState.h"

#include "EngineUtils.h"
 
ARTSPlayerController::ARTSPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
	this->bEnableClickEvents = true;
	this->bEnableAutoLODGeneration = true;
	FOWManagerClass = AFogOfWarManager::StaticClass();
}

void ARTSPlayerController::BeginPlay()
{
	Super::BeginPlay();

	/*This is carryover from the lobby UI set it back so that the initial click is used*/
	FInputModeGameOnly InputMode;
	InputMode.SetConsumeCaptureMouseDown(false);
	SetInputMode(InputMode);
}

void ARTSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	ClickEventKeys.Add(EKeys::RightMouseButton);
	ClickEventKeys.Add(EKeys::LeftMouseButton);
}

void ARTSPlayerController::SetPawn(APawn * InPawn)
{
	Super::SetPawn(InPawn);
	
	if (Cast<ADefaultPlayerState>(PlayerState) && Cast<ACommander>(InPawn))
	{
		int team_id = Cast<ADefaultPlayerState>(PlayerState)->TeamID;
		Cast<ACommander>(InPawn)->SetTeam(team_id);
	}	
}

void ARTSPlayerController::ClientNotifyTeamChange(int newteam)
{
	Super::ClientNotifyTeamChange(newteam);

	/*Clear the units we currently have under our control*/
	SelectedUnits.Empty();
	SelectedStructures.Empty();

}

void ARTSPlayerController::FinishLocalPlayerSetup()
{
	UWorld* world = GetWorld();
	if (IsValid(world))
	{
		for (TActorIterator<ARTSMinion> It(world); It; ++It)
		{
			ARTSMinion* Minion = *It;
			if (IsValid(Minion) && !Minion->IsPendingKill())
			{
				Minion->SetDeselected();
				const int tid = GetTeamID();
				const int mid = Minion->GetTeam();

				if (mid != tid)
				{
					Minion->SetTeamColors(FLinearColor::Red);
					Minion->SetSelected();
				}
			}
		}
	}
}

AFogOfWarManager * ARTSPlayerController::InitFOW()
{
	ADefaultPlayerState * PS = Cast<ADefaultPlayerState>(PlayerState);

	/*Server Will spawn the FOWManager*/
	if (!FOWManager)
	{
		FActorSpawnParameters SpawnParams;
		UWorld * World = GetWorld();

		if (World && FOWManagerClass)
		{
			FOWManager = World->SpawnActor<AFogOfWarManager>(FOWManagerClass, SpawnParams);
		}
	}

	return(FOWManager);
}



bool ARTSPlayerController::MoveMinions_Validate(ARTSPlayerController * PC, const TArray<ARTSMinion *> &Units, FHitResult Hit)
{
	return (true);
}

void ARTSPlayerController::MoveMinions_Implementation(ARTSPlayerController * PC, const TArray<ARTSMinion *> &Units, FHitResult Hit)
{
	AActor * target = Hit.GetActor();
	for (int i = 0; i < Units.Num(); i++)
	{
		if (!Units[i] || Units[i]->IsPendingKill() || !Units[i]->IsAlive()) /*Skip Dead or Invalid Minions*/
		{

		}
		else if (!Units[i]->GetCommander()) /*Unit is or has a commander, notify him instead*/
		{

			if (Cast<ARTSMinion>(target) || Cast<IRTSObjectInterface>(target))
			{
				Units[i]->SetTarget(target);
			}
			else
			{
				FVector MoveLocal = Hit.Location + FVector(i / 2 * 100, i % 2 * 100, 0);

				Units[i]->ClearTarget(); /*Unit might be doing something, if he is, clear internal data*/
				Units[i]->ReleaseAssets();
				Units[i]->RtsMove(MoveLocal);
			}
		}
		else //Notify the Commander of the new Target
		{

		}
	}
}

bool ARTSPlayerController::ServerPurchaseRTSObject_Validate(ARTSStructure * SpawningStructure, TSubclassOf<UObject> RequestedClass)
{
	return(true);
}

void ARTSPlayerController::ServerPurchaseRTSObject_Implementation(ARTSStructure * SpawningStructure, TSubclassOf<UObject> RequestedClass)
{
	UWorld* world = GetWorld();
	check(world);

	ARTFPSGameState* gs = world->GetGameState<ARTFPSGameState>();

	if (gs && gs->PurchaseUnit(RequestedClass, this))
	{
		SpawningStructure->QueueActor(RequestedClass, this);
	}
}

bool ARTSPlayerController::ServerPurchaseStructure_Validate(TSubclassOf<AActor> RequestedClass, FTransform BuildLocation)
{
	return(true);
}

void ARTSPlayerController::ServerPurchaseStructure_Implementation(TSubclassOf<AActor> RequestedClass, FTransform BuildLocation)
{
	UWorld* world = GetWorld();
	check(world);

	ARTFPSGameState* gs = world->GetGameState<ARTFPSGameState>();

	if (gs && gs->PurchaseUnit(RequestedClass,this))
	{
		gs->HandleStructureSpawn(RequestedClass, BuildLocation, this);
	}
}