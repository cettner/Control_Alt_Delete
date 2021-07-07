// Fill out your copyright notice in the Description page of Project Settings.


#include "ClaimableSquareGameGrid.h"
#include "GridModifierType.h"
#include "GridClaimingActor.h"

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
	int32 gridid = GetUniqueGridID(TileLocation);
	if (gridid != INVALID_TILE_ID)
	{
		return(GridModifiers[gridid].TileModifiers);
	}

	return (TArray<UGridModifierType*>());
}

bool AClaimableSquareGameGrid::ApplyModifier(UGridModifierType* Modifier, FGridTile TileLocation,  AGridClaimingActor * Source)
{
	bool success = false;
	int32 gridid = GetUniqueGridID(TileLocation);
	if (Modifier != nullptr && gridid != INVALID_TILE_ID)
	{
		Modifier->ApplyModifier(this, TileLocation, Source);
		GridModifiers[gridid].TileModifiers.Emplace(Modifier);
		success = true;
	}
	return(success);
}

bool AClaimableSquareGameGrid::ApplyModifier(UGridModifierType* Modifier, TArray<FGridTile> TileLocations,  AGridClaimingActor * Source)
{
	bool success = true;
	for(int i = 0; i < TileLocations.Num(); i++)
	{
		success &= ApplyModifier(Modifier, TileLocations[i], Source);
	}
	return(success);
}

bool AClaimableSquareGameGrid::RemoveModifier(UGridModifierType* Modifier, FGridTile TileLocation,  AGridClaimingActor * Source)
{
	bool success = false;
	int32 gridid = GetUniqueGridID(TileLocation);
	
	if ((Modifier != nullptr) && (gridid != INVALID_TILE_ID))
	{
		success = (GridModifiers[gridid].TileModifiers.RemoveSingle(Modifier) == 1U);
		success &= Modifier->RemoveModifier(this, TileLocation, Source);
	}

	return(success);
}

bool AClaimableSquareGameGrid::RemoveModifier(UGridModifierType* Modifier, TArray<FGridTile> TileLocations, AGridClaimingActor* Source)
{
	bool success = true;
	for (int i = 0; i < TileLocations.Num(); i++)
	{
		success &= RemoveModifier(Modifier, TileLocations[i], Source);
	}
	return(success);
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

#if WITH_EDITOR
void AClaimableSquareGameGrid::OnConstruction(const FTransform & Transform)
{
	Super::OnConstruction(Transform);
	SimulateGrid();
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
#endif