// Fill out your copyright notice in the Description page of Project Settings.


#include "ClaimableSquareGameGrid.h"
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
	return false;
}

bool AClaimableSquareGameGrid::MoveGridActor(AGridClaimingActor * InActor, FGridTile TileLocation)
{
	return false;
}

bool AClaimableSquareGameGrid::CanMoveTo(AGridClaimingActor * InActor, FGridTile TileLocation)
{
	if (!InActor) return false;
	bool success = true;

	TArray<FGridTileOffset> attemptedclaimspace = InActor->GetRelativeClaimSpace();
	TArray<FGridTile> gridtiles;

	success &= GetGridTilesFromOffset(InActor->GetRootGridTile(), attemptedclaimspace, gridtiles, true);

	return success;
}

TArray<UGridModifierType *> AClaimableSquareGameGrid::GetActiveModifiers(FGridTile TileLocation) const
{
	TArray<UGridModifierType *> Mods = TArray<UGridModifierType *>();


	return (Mods);
}

bool AClaimableSquareGameGrid::ApplyModifier(FGridTile TileLocation, UGridModifierType * Modifier, AGridClaimingActor * Source)
{
	if (Modifier != nullptr)
	{
		Modifier->ApplyModifier(this, TileLocation, Source);
	}
	return(true);
}

bool AClaimableSquareGameGrid::ApplyModifier(TArray<FGridTile> TileLocations, UGridModifierType * Modifier, AGridClaimingActor * Source)
{
	if (Modifier != nullptr)
	{
		Modifier->ApplyModifier(this, TileLocations, Source);
	}
	return(true);
}

bool AClaimableSquareGameGrid::RemoveModifier(FGridTile TileLocation, UGridModifierType * Modifier, AGridClaimingActor * Source)
{
	bool retval = false;
	if (Modifier)
	{
		retval = Modifier->OnModifierRemoved(this, TileLocation, Source);
	}

	return(retval);
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
