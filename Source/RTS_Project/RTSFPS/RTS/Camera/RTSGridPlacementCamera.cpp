// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSGridPlacementCamera.h"
#include "Kismet/GameplayStatics.h"


void ARTSGridPlacementCamera::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	bOnlyRelevantToOwner = true;
	if (!IsPlacingActor())
	{
		PlacementActor = CreatePlacementActor(StructureClass);
	}

}

void ARTSGridPlacementCamera::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	InputComponent->BindAction("BKey", IE_Pressed, this, &ARTSGridPlacementCamera::ToggleBuildGrid);
	//InputComponent->BindAction("LeftMouse", IE_Released, this, &ARTSSelectionCamera::SelectReleased);

}

APlacementActor * ARTSGridPlacementCamera::CreatePlacementActor(const TSubclassOf<AActor> InActorClass, FTransform SpawnTransform) const
{
	UWorld * World = GetWorld();
	if (World == nullptr) return nullptr;

	APlacementActor * placeactor = nullptr;


	placeactor = World->SpawnActorDeferred<APlacementActor>(PlacementActorClass, SpawnTransform);
	placeactor->PreInitializePlacementActor(InActorClass, SpawnTransform);
	UGameplayStatics::FinishSpawningActor(placeactor, SpawnTransform);

	return(placeactor);
}

APlacementActor * ARTSGridPlacementCamera::GetPlacementActor() const
{
	return PlacementActor;
}

bool ARTSGridPlacementCamera::IsPlacingActor() const
{
	return (PlacementActor != nullptr);
}

ASquareGameGrid * ARTSGridPlacementCamera::GetCurrentGrid() const
{
	if (PlacementActor == nullptr) return nullptr;

	return(PlacementActor->GetParentGrid());

}

void ARTSGridPlacementCamera::ToggleBuildGrid()
{
	if (bIsBuildGridVisible)
	{
		PlacementActor->GetParentGrid()->HideGrid();
		bIsBuildGridVisible = false;
	}
	else
	{
		PlacementActor->GetParentGrid()->ShowGrid();
		bIsBuildGridVisible = true;
	}

}


void ARTSGridPlacementCamera::PreInitializeGridActor(AGridAttatchmentActor* GridActor, const TSubclassOf<AActor> InActorClass, FTransform SpawnTransform) const
{
}

void ARTSGridPlacementCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!IsPlacingActor()) return;

	APlayerController * pc = GetController<APlayerController>();
	if (pc == nullptr) return;

	FHitResult hit;
	pc->GetHitResultUnderCursor(GRID_CHANNEL, false, hit);

	
	ASquareGameGrid * currentgrid = GetCurrentGrid();

	FGridTile hittile = currentgrid->GetTileFromLocation(hit.Location);

	FGridTile actortile = PlacementActor->GetRootGridTile();
	int32 tileid = currentgrid->GetUniqueGridID(actortile);


	if (hittile.IsValid && hit.bBlockingHit && !(PlacementActor->GetRootGridTile() == hittile))
	{
		PlacementActor->SetTileLocation(hittile);

		if (PlacementActor->IsPlaceable())
		{
			PlacementActor->SetMeshColor(FLinearColor::Green);
		}
		else
		{
			PlacementActor->SetMeshColor(FLinearColor::Red);
		}
	}
}
