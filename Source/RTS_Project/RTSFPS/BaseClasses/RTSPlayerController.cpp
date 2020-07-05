// Fill out your copyright notice in the Description page of Project Settings.

#include "RTSPlayerController.h"
#include "ConstructorHelpers.h"
#include "RTSFPS/RTS/Structures/RTSStructure.h"
#include "GameArchitecture/Game/RTFPSPlayerState.h"
#include "RTSFPS/FPS/Commander.h"
#include "AssetHelpers/GameAssets.h"
#include "ConstructorHelpers.h"
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
	bShowMouseCursor = true;
	HudPtr = Cast<ARTSHUD>(GetHUD());

	FInputModeGameOnly InputMode;
	InputMode.SetConsumeCaptureMouseDown(false);
	SetInputMode(InputMode);

	if (Cast<ACommander>(GetPawn()) && HudPtr)
	{
		bShowMouseCursor = false;
		HudPtr->ChangeHUDState(ARTSHUD::FPS_AIM_AND_SHOOT);
	}
	else if (Cast<ARTSCamera>(GetPawn()) && HudPtr)
	{
		HudPtr->ChangeHUDState(ARTSHUD::RTS_SELECT_AND_MOVE);
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Invalid Pawn!")));
	}

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
	HudPtr->ChangeHUDState(ARTSHUD::FPS_AIM_AND_SHOOT);
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
	HudPtr->ChangeHUDState(ARTSHUD::RTS_SELECT_AND_MOVE);
	Possess(camera);
}

