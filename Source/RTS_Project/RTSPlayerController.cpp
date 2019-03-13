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
			Update_UI_Spawn(SpawnedMine);
		}
		else if(type == SPAWNER)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			ARTSStructure * SpawnedSpawner = World->SpawnActor<ARTSStructure>(Spawner, Location, Rotation, SpawnParams);
			Update_UI_Spawn(SpawnedSpawner);
		}
	}
}

void ARTSPlayerController::PossessCommander(ACommander * commander)
{
	bShowMouseCursor = false;
	HudPtr->Change_HUD_State(ARTSHUD::FPS_AIM_AND_SHOOT);
	state = FPS_MODE;
	Possess(commander);
}

void ARTSPlayerController::PossessRTSCamera(ARTSCamera * camera)
{
	bShowMouseCursor = true;
	HudPtr->Change_HUD_State(ARTSHUD::RTS_SELECT_AND_MOVE);
	state = RTS_MODE;
	Possess(camera);
}

void ARTSPlayerController::BeginPlay()
{
	bShowMouseCursor = true;
	HudPtr = Cast<ARTSHUD>(GetHUD());  // GetHud only returns AHUD, need to cast to get child type, ARTSHUD

	Resource_Count.Add(ResourceOnePLayerStart);
	Resource_Count.Add(ResourceTwoPLayerStart);
	Resource_Count.Add(ResourceThreePLayerStart);
	Update_UI_Resource();
	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Resources starting out at 0:%d, 1:%d, and 2:%d!"),ResourceOnePLayerStart,ResourceTwoPLayerStart,ResourceThreePLayerStart));

}

void ARTSPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAction("LeftMouse", IE_Pressed, this, &ARTSPlayerController::SelectPressed);
	InputComponent->BindAction("LeftMouse", IE_Released, this, &ARTSPlayerController::SelectReleased);
	InputComponent->BindAction("RightMouse", IE_Pressed, this, &ARTSPlayerController::MoveSelected);
	
	ClickEventKeys.Add(EKeys::RightMouseButton);
	ClickEventKeys.Add(EKeys::LeftMouseButton);

}

void ARTSPlayerController::SelectPressed()
{
	FHitResult Hit;
	GetHitResultUnderCursor(SELECTION_CHANNEL, false, Hit);
	AActor * target = Hit.GetActor();

	if (target)
	{
		int debug = 90;
	}

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
		HudPtr->Selected_Structure.Empty();
		SelectedStructures.Empty();
		SelectedUnits = HudPtr->Selected_Units;
	}
	else if (HudPtr->StructureSelected)
	{
		HudPtr->StructureSelected = false;
		SelectedStructures = HudPtr->Selected_Structure;
	

		HudPtr->Selected_Units.Empty();
		SelectedUnits.Empty();
	}

	//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Units: %d,Structures: %d"), SelectedUnits.Num(), SelectedStructures.Num()));
	Update_UI_Selection();
}

void ARTSPlayerController::MoveSelected()
{
	if (SelectedUnits.Num() > 0)
	{
		for (int32 i = 0; i < SelectedUnits.Num(); i++)
		{
			FHitResult Hit;
			GetHitResultUnderCursor(SELECTION_CHANNEL, false, Hit);
			AActor * target = Hit.GetActor();

			if (target)
			{
				SelectedUnits[i]->SetTarget(target);
			}
			/*
			FVector MoveLocal = Hit.Location + FVector(i / 2 * 100, i % 2 * 100, 0);

			if (SelectedUnits[i]->HasAssets())
			{
				SelectedUnits[i]->ReleaseAssets(MoveLocal);
			}
			else
			{
				SelectedUnits[i]->RtsMove(MoveLocal);
			}
			*/
		}
	}
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
