// Fill out your copyright notice in the Description page of Project Settings.

#include "RTSPlayerController.h"
#include "ConstructorHelpers.h"
#include "RTSStructure.h"
#include "DefaultPlayerState.h"
#include "Weapon.h"
#include "Commander.h"
#include "Engine.h"

ARTSPlayerController::ARTSPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
	this->bEnableClickEvents = true;
	this->bEnableAutoLODGeneration = true;
}

void ARTSPlayerController::BeginPlay()
{
	bShowMouseCursor = true;
	HudPtr = Cast<ARTSHUD>(GetHUD());

	if (!HasAuthority())
	{
		if (Cast<ACommander>(GetPawn()) && HudPtr)
		{
			bShowMouseCursor = false;
			HudPtr->Change_HUD_State(ARTSHUD::FPS_AIM_AND_SHOOT);
		}
		else if (Cast<ARTSCamera>(GetPawn()) && HudPtr)
		{
			HudPtr->Change_HUD_State(ARTSHUD::RTS_SELECT_AND_MOVE);
		}
		else
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Invalid Pawn!")));
		}
	}

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
		Cast<ACommander>(InPawn)->team_index = Cast<ADefaultPlayerState>(PlayerState)->Team_ID;
	}
	
}

ARTSStructure * ARTSPlayerController::Spawn_RTS_Structure(FVector Location, FRotator Rotation, int Structure_index)
{
	Structure_Types type = (Structure_Types)Structure_index;
	UWorld* const World = GetWorld();
	
	ARTSStructure * BuiltStructure = SpawnHelper.SpawnStructure(type,Location,Rotation,World);
	return(BuiltStructure);
}

ARTSMinion * ARTSPlayerController::Spawn_RTS_Minion(FVector Location, FRotator Rotation, int Unit_index)
{
	Unit_Types type = (Unit_Types)Unit_index;
	UWorld* const World = GetWorld();

	ARTSMinion * SpawnedMinion = SpawnHelper.SpawnMinion(type,Location,Rotation,World);
	return(SpawnedMinion);
}

AWeapon * ARTSPlayerController::Spawn_Weapon(FVector Location, FRotator Rotation, int Weapon_index)
{
	Weapon_Types type = (Weapon_Types)Weapon_index;
	UWorld* const World = GetWorld();

	AWeapon * SpawnedWeapon = SpawnHelper.SpawnWeapon(type,Location,Rotation,World);
	return(SpawnedWeapon);
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
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Blue, FString::Printf(TEXT("Moving %d, Im team %d"),Units[i]->team_index, PS->Team_ID));
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
	HudPtr->Change_HUD_State(ARTSHUD::FPS_AIM_AND_SHOOT);
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
	HudPtr->Change_HUD_State(ARTSHUD::RTS_SELECT_AND_MOVE);
	Possess(camera);
}


bool ARTSPlayerController::FinishPlayerLogin_Validate()
{
	return(true);
}

void ARTSPlayerController::FinishPlayerLogin_Implementation()
{

}
