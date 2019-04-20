// Fill out your copyright notice in the Description page of Project Settings.

#include "RTSPlayerController.h"
#include "ConstructorHelpers.h"
#include "RTSStructure.h"
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


void ARTSPlayerController::PossessCommander(ACommander * commander)
{
	bShowMouseCursor = false;
	HudPtr->Change_HUD_State(ARTSHUD::FPS_AIM_AND_SHOOT);
	Possess(commander);
}

void ARTSPlayerController::PossessRTSCamera(ARTSCamera * camera)
{
	bShowMouseCursor = true;
	HudPtr->Change_HUD_State(ARTSHUD::RTS_SELECT_AND_MOVE);
	Possess(camera);
}

void ARTSPlayerController::BeginPlay()
{
	bShowMouseCursor = true;
	HudPtr = Cast<ARTSHUD>(GetHUD());

	Resource_Count.Add(ResourceOnePLayerStart);
	Resource_Count.Add(ResourceTwoPLayerStart);
	Resource_Count.Add(ResourceThreePLayerStart);
	Update_UI_Resource();
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Resources starting out at 0:%d, 1:%d, and 2:%d!"),ResourceOnePLayerStart,ResourceTwoPLayerStart,ResourceThreePLayerStart));

}

void ARTSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	ClickEventKeys.Add(EKeys::RightMouseButton);
	ClickEventKeys.Add(EKeys::LeftMouseButton);

}



void ARTSPlayerController::AddResource(int amount_to_add, Resource_Types type)
{
	Resource_Count[type] += amount_to_add;

	if (Resource_Count[type] > MAX_RESOURCES)
	{
		Resource_Count[type] = MAX_RESOURCES;
	}
	Update_UI_Resource();
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Added %d of resource type %d totals now: 1: %d, 2: %d, and 3: %d!"),amount_to_add,(int)type+1,Resource_Count[Resource_Types::TYPE_ONE],Resource_Count[Resource_Types::TYPE_TWO], Resource_Count[Resource_Types::TYPE_THREE]));
}

bool ARTSPlayerController::TakeResource(int amount_to_take, Resource_Types type)
{
	if (amount_to_take > Resource_Count[type])
	{
		return false;
	}
	else
	{
		Resource_Count[type] -= amount_to_take;
		Update_UI_Resource();
		return true;
	}
}
