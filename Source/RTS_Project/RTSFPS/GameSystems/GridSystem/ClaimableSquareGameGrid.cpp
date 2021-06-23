// Fill out your copyright notice in the Description page of Project Settings.


#include "ClaimableSquareGameGrid.h"
#include "GridClaimingActor.h"

void AClaimableSquareGameGrid::OnConstruction(const FTransform & Transform)
{
	Super::OnConstruction(Transform);
	SimulateGrid();
}

bool AClaimableSquareGameGrid::AddGridActor(AGridClaimingActor * InActor, FGridTile TileLocation, bool TickOnAdd)
{
	if (!InActor) return false;
	bool success = true;

	TArray<FGridTileOffset> attemptedclaimspace = InActor->GetRelativeClaimSpace();
	TArray<FGridTile> gridtiles;

	success &= GetGridTilesFromOffset(InActor->GetRootGridTile(), attemptedclaimspace, gridtiles, true);

	if (success)
	{
		InActor->SetGridClaimSpace(gridtiles, this);
		GridActors.AddUnique(InActor);
	}


	return success;
}

bool AClaimableSquareGameGrid::RemoveGridActor(AGridClaimingActor * InActor, bool TickOnRemoval)
{
	bool success = (GridActors.Remove(InActor) > 0U);
	if (GridActors.Contains(InActor))
	{
		InActor->SetGridClaimSpace(TArray<FGridTile>(), nullptr);
	}

	return (success);
}

bool AClaimableSquareGameGrid::MoveGridActor(AGridClaimingActor * InActor, FGridTile TileLocation)
{
	bool success = false;
	if (InActor == nullptr) return success;

	TArray<FGridTileOffset> offsets = InActor->GetRelativeClaimSpace();

	TArray<FGridTile> outtiles = TArray<FGridTile>();
		
	success = GetGridTilesFromOffset(TileLocation, offsets, outtiles);

	InActor->SetGridClaimSpace(outtiles,this);

	return success;
}

bool AClaimableSquareGameGrid::CanMoveTo(AGridClaimingActor * InActor, FGridTile TileLocation)
{
	if (!InActor) return false;
	bool success = true;

	TArray<FGridTileOffset> attemptedclaimspace = InActor->GetRelativeClaimSpace();
	TArray<FGridTile> gridtiles;

	success &= GetGridTilesFromOffset(TileLocation, attemptedclaimspace, gridtiles, true);

	return success;
}

TArray<UGridModifierType *> AClaimableSquareGameGrid::GetActiveModifiers(FGridTile TileLocation) const
{
	TArray<UGridModifierType *> Mods = TArray<UGridModifierType *>();


	return (Mods);
}

bool AClaimableSquareGameGrid::ApplyModifier(UGridModifierType* Modifier, FGridTile TileLocation,  AGridClaimingActor * Source)
{
	if (Modifier != nullptr)
	{
		Modifier->ApplyModifier(this, TileLocation, Source);
	}
	return(true);
}

bool AClaimableSquareGameGrid::ApplyModifier(UGridModifierType* Modifier, TArray<FGridTile> TileLocations,  AGridClaimingActor * Source)
{
	if (Modifier != nullptr)
	{
		Modifier->ApplyModifier(this, TileLocations, Source);
	}
	return(true);
}

bool AClaimableSquareGameGrid::RemoveModifier(UGridModifierType* Modifier, FGridTile TileLocation,  AGridClaimingActor * Source)
{
	bool retval = false;
	if (Modifier)
	{
		retval = Modifier->OnModifierRemoved(this, TileLocation, Source);
	}

	return(retval);
}

bool AClaimableSquareGameGrid::RemoveModifier(UGridModifierType* Modifier, TArray<FGridTile> TileLocations, AGridClaimingActor* Source)
{
	bool retval = false;
	if (Modifier)
	{
		retval = Modifier->OnModifierRemoved(this, TileLocations, Source);
	}

	return(retval);
}

void AClaimableSquareGameGrid::SimulateGrid()
{
	HideSelectedTile(GridData);

	for (int i = 0; i < GridActors.Num(); i++)
	{
		GridActors[i]->SimulateModfiers();
	}
}

bool AClaimableSquareGameGrid::ISSimulatingEffects() const
{
	UWorld * World = GetWorld();
	if (World == nullptr) return false;

	EWorldType::Type worldtype = World->WorldType;

	if (worldtype == EWorldType::EditorPreview || worldtype == EWorldType::GamePreview || worldtype == EWorldType::Editor)
	{
		return(true);
	}

	return(false);
}

bool AClaimableSquareGameGrid::BuildGridData()
{
	bool success = Super::BuildGridData();
	for (int i = 0; i < GridData.Num(); i++)
	{
		FGridModifierSet EmptySet = FGridModifierSet();
		GridModifiers.Emplace(EmptySet);
	}

	success &= (GridModifiers.Num() == GridData.Num());
	return (success);
}

bool AClaimableSquareGameGrid::GetGridTilesFromOffset(FGridTile StartTile, TArray<FGridTileOffset> Offsets, TArray<FGridTile>& OutTiles, bool bisstartinclusive) const
{
	if(!StartTile.IsValid) return false;
	
	bool success = true;

	for(int i = 0; i < Offsets.Num(); i++)
	{
		FGridTile newtile = FGridTile();
		newtile.row = StartTile.row + Offsets[i].RowOffset;
		newtile.column = StartTile.column + Offsets[i].ColOffset;

		uint32_t gridid = GetUniqueGridID(newtile);
		if(gridid != INVALID_TILE_ID)
		{
			OutTiles.AddUnique(GridData[gridid]);
		}
		else
		{
			success = false;
		}
	}

	if(bisstartinclusive)
	{
		OutTiles.AddUnique(StartTile);
	}

	return(success);
}
