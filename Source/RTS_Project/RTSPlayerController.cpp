// Fill out your copyright notice in the Description page of Project Settings.

#include "RTSPlayerController.h"
#include "ConstructorHelpers.h"
#include "RTSStructure.h"
#include "Engine.h"

ARTSPlayerController::ARTSPlayerController()
{
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
	this->bEnableClickEvents = true;
	this->bEnableAutoLODGeneration = true;

	static ConstructorHelpers::FObjectFinder<UBlueprint> MineBlueprint(TEXT("Blueprint'/Game/TopDownBP/Actors/Structures/MINE_BP.MINE_BP'"));
	static ConstructorHelpers::FObjectFinder<UBlueprint> SpawnerBlueprint(TEXT("Blueprint'/Game/TopDownBP/Actors/Structures/Spawner_BP.Spawner_BP'"));

	if (MineBlueprint.Object)
	{
		Mine = (UClass*)MineBlueprint.Object->GeneratedClass;
	}
	else
	{
	
	}

	if (SpawnerBlueprint.Object)
	{
		Spawner = (UClass*)SpawnerBlueprint.Object->GeneratedClass;
	}
	else
	{
	
	}

}

void ARTSPlayerController::Spawn_RTS_Structure(FVector Location, FRotator Rotation, int Structure_index)
{
	UWorld* const World = GetWorld();
	if (Structure_index > (int)STRUCTURELBOUND && Structure_index < (int)STRUCTUREUBOUND && World)
	{
		Structure_Types type = (Structure_Types)Structure_index;

		if (type == MINE)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			ARTSStructure * SpawnedMine = World->SpawnActor<ARTSStructure>(Mine, Location, Rotation, SpawnParams);
		}
		else if(type == SPAWNER)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			ARTSStructure * SpawnedSpawner = World->SpawnActor<ARTSStructure>(Spawner, Location, Rotation, SpawnParams);
		}
	}
}

void ARTSPlayerController::BeginPlay()
{
	HudPtr = Cast<ARTSHUD>(GetHUD());  // GetHud only returns AHUD, need to cast to get child type, ARTSHUD

	Resource_Count.Add(ResourceOnePLayerStart);
	Resource_Count.Add(ResourceTwoPLayerStart);
	Resource_Count.Add(ResourceThreePLayerStart);

}

void ARTSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("LeftMouse", IE_Pressed, this, &ARTSPlayerController::SelectPressed);
	InputComponent->BindAction("LeftMouse", IE_Released, this, &ARTSPlayerController::SelectReleased);
	InputComponent->BindAction("RightMouse", IE_Pressed, this, &ARTSPlayerController::MoveSelected);
	
	ClickEventKeys.Add(EKeys::RightMouseButton);
	ClickEventKeys.Add(EKeys::LeftMouseButton);

	// switch to other character hud used for debugging, supported by level blueprint
	InputComponent->BindAction("KeyOne", IE_Pressed, this, &ARTSPlayerController::SwapHud);
}

void ARTSPlayerController::SelectPressed()
{
	if (!HudPtr->StructureSelected)
	{
		HudPtr->Initial_select = HudPtr->GetMouseLocation();
		HudPtr->SelctionInProcess = true;
	}
}

void ARTSPlayerController::SelectReleased()
{
	if (HudPtr->SelctionInProcess)
	{
		HudPtr->SelctionInProcess = false;
		SelectedUnits = HudPtr->Selected_Units;
	}
	else if (HudPtr->StructureSelected)
	{
		HudPtr->StructureSelected = false;
		SelectedStructures = HudPtr->Selected_Structure;
	}
	Update_UI_Selection();
}

void ARTSPlayerController::MoveSelected()
{
	if (SelectedUnits.Num() > 0)
	{
		for (int32 i = 0; i < SelectedUnits.Num(); i++)
		{
			FHitResult Hit;
			GetHitResultUnderCursor(ECollisionChannel::ECC_Visibility, false, Hit);
			FVector MoveLocal = Hit.Location + FVector(i / 2 * 100, i % 2 * 100, 0);

			if (SelectedUnits[i]->HasAssets())
			{
				SelectedUnits[i]->ReleaseAssets(MoveLocal);
			}
			else
			{
				SelectedUnits[i]->RtsMove(MoveLocal);
			}
		}
	}
}

void ARTSPlayerController::SwapHud()
{
	HudPtr->bIsRTSCharacter = !HudPtr->bIsRTSCharacter;
}

void ARTSPlayerController::AddResource(int amount_to_add, Resource_Types type)
{
	Resource_Count[type] += amount_to_add;

	if (Resource_Count[type] > MAX_RESOURCES)
	{
		Resource_Count[type] = MAX_RESOURCES;
	}
	Update_UI_Resource();
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
