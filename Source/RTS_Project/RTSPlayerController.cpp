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

	static ConstructorHelpers::FObjectFinder<UBlueprint> MineBlueprint(TEXT("Blueprint'/Game/TopDownBP/Actors/MINE_BP.MINE_BP'"));

	if (MineBlueprint.Object)
	{
		Mine = (UClass*)MineBlueprint.Object->GeneratedClass;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, FString::Printf(TEXT("Failed to Load Mine Asset!")));
	}
}

void ARTSPlayerController::Spawn_RTS_Structure(FVector Location, FRotator Rotation, int Structure_index)
{
	UWorld* const World = GetWorld();
	if (Structure_index > (int)LBOUND && Structure_index < (int)UBOUND && World)
	{
		Structure_Types type = (Structure_Types)Structure_index;

		if (type == MINE)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			ARTSStructure * SpawnedMine = World->SpawnActor<ARTSStructure>(Mine, Location, Rotation, SpawnParams);
		}
	}
}

void ARTSPlayerController::BeginPlay()
{
	HudPtr = Cast<ARTSHUD>(GetHUD());  // GetHud only returns AHUD, need to cast to get child type, ARTSHUD

	Resource_Count.Add(ResourceOnePLayerStart);
	Resource_Count.Add(ResourceTwoPLayerStart);
	Resource_Count.Add(ResourceThreePLayerStart);
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Resources starting out at 0:%d, 1:%d, and 2:%d!"),ResourceOnePLayerStart,ResourceTwoPLayerStart,ResourceThreePLayerStart));

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
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, FString::Printf(TEXT("Added %d of resource type %d totals now: 1: %d, 2: %d, and 3: %d!"),amount_to_add,(int)type+1,Resource_Count[Resource_Types::TYPE_ONE],Resource_Count[Resource_Types::TYPE_TWO], Resource_Count[Resource_Types::TYPE_THREE]));
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
