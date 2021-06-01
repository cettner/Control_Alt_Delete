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
	}


	return false;
}

bool AClaimableSquareGameGrid::RemoveGridActor(AGridClaimingActor * InActor, bool TickOnRemoval)
{
	return false;
}

bool AClaimableSquareGameGrid::MoveGridActor(AGridClaimingActor * InActor, FGridTile TileLocation)
{
	return false;
}

TArray<TSubclassOf<UGridModifierType>> AClaimableSquareGameGrid::GetActiveModifiers(FGridTile TileLocation) const
{
	TArray<TSubclassOf<UGridModifierType>> Mods = TArray<TSubclassOf<UGridModifierType>>();


	return (Mods);
}

void AClaimableSquareGameGrid::OnModifierApplied(FGridTile TileLocation, TSubclassOf<UGridModifierType>, AGridClaimingActor * Source)
{

}

bool AClaimableSquareGameGrid::GetGridTilesFromOffset(FGridTile StartTile, TArray<FGridTileOffset> Offsets, TArray<FGridTile>& OutTiles, bool bisstartinclusive) const
{
	if(!StartTile.IsValid) return false;
	
	bool success = true;

	for(int i = 0; i < Offsets.Num(); i++)
	{
		FGridTile newtile = FGridTile();
		newtile.row = StartTile.row + Offsets[i].row;
		newtile.column = StartTile.column + Offsets[i].column;

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
