// Fill out your copyright notice in the Description page of Project Settings.

#include "RTSPlayerController.h"
#include "RTS_Project/RTSFPS/RTS/Structures/RTSStructure.h"
#include "RTS_Project/GameArchitecture/Game/RTFPSPlayerState.h"
#include "RTS_Project/RTSFPS/FPS/Commander.h"
#include "RTS_Project/AssetHelpers/GameAssets.h"

#include "Engine.h"
 
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
	

	/*Client or Listen Server, Initilize the Hud Locally*/
    /*Initialize Hud*/
	HudPtr = Cast<ARTSHUD>(GetHUD());
	if (HudPtr)
	{
		if (Cast<ACommander>(GetPawn()))
		{
			bShowMouseCursor = false;
			HudPtr->ChangeHUDState(HUDSTATE::FPS_AIM_AND_SHOOT);
		}
		else if (Cast<ARTSCamera>(GetPawn()))
		{
			HudPtr->ChangeHUDState(HUDSTATE::RTS_SELECT_AND_MOVE);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Invalid Pawn!")));
		}
		HudPtr->InitializeUI();
	}
}

void ARTSPlayerController::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void ARTSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	ClickEventKeys.Add(EKeys::RightMouseButton);
	ClickEventKeys.Add(EKeys::LeftMouseButton);
	InputComponent->BindAction("Debug", IE_Pressed, this, &ARTSPlayerController::DebugEvent);
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

	UWorld* World = GetWorld();
	if (World == nullptr) return;

	/*Iterate through all minions in the map, set team color on all minons that don't match our team ID*/
	for (TActorIterator<ARTSMinion> It(World); It; ++It)
	{
		ARTSMinion* Minion = *It;
		if (Minion && !Minion->IsPendingKill())
		{
			Minion->SetDeselected();
			if (Minion->GetTeam() != newteam)
			{
				Minion->SetTeamColors();
				Minion->SetSelected();
			}
		}
	}
}

void ARTSPlayerController::FinishLocalPlayerSetup(ARTFPSPlayerState * PS)
{
	if (PS == nullptr) return;

	TArray<AActor *> Units;
	if (PS->isRtsPlayer && FOWManager)
	{
		for (TObjectIterator<ARTSMinion> Itr; Itr; ++Itr)
		{
			ARTSMinion * freeminion = *Itr;
			if (freeminion->GetTeam() == PS->TeamID && freeminion->GetLocalRole() != ROLE_Authority)
			{
				Units.AddUnique(freeminion);
			}
		}
		FOWManager->EnableFOW(Units);
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

void ARTSPlayerController::DebugEvent()
{
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
		if (!Units[i]->GetCommander()) /*Unit is or has a commander, notify him instead*/
		{
			 ADefaultPlayerState * PS = Cast<ADefaultPlayerState>(PlayerState);
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, FString::Printf(TEXT("Moving %d, Im team %d"),Units[i]->GetTeam(), PS->TeamID));
			if (Cast<ARTSMinion>(target) || Cast<ARTSSelectable>(target))
			{
				Units[i]->SetTarget(target);
			}
			else
			{
				FVector MoveLocal = Hit.Location + FVector(i / 2 * 100, i % 2 * 100, 0);

				Units[i]->ClearTarget(); /*Unit might be doing something, if he is, clear internal data*/
				Units[i]->RtsMove(MoveLocal);
			}
		}
		else //Notify the Commander of the new Target
		{

		}
	}
}

bool ARTSPlayerController::PossessCommander_Validate(ACommander * commander)
{
	if (commander && HudPtr)
	{
		return(true);
	}
	else
	{
		return(false);
	}
}

void ARTSPlayerController::PossessCommander_Implementation(ACommander * commander)
{
	bShowMouseCursor = false;
	HudPtr->ChangeHUDState(HUDSTATE::FPS_AIM_AND_SHOOT);
	Possess(commander);
}

bool ARTSPlayerController::PossessRTSCamera_Validate(ARTSCamera * camera)
{
	if (camera && HudPtr)
	{
		return(true);
	}
	else
	{
		return(false);
	}
}

void ARTSPlayerController::PossessRTSCamera_Implementation(ARTSCamera * camera)
{
	bShowMouseCursor = true;
	HudPtr->ChangeHUDState(HUDSTATE::RTS_SELECT_AND_MOVE);
	Possess(camera);
}

