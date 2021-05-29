// Fill out your copyright notice in the Description page of Project Settings.


#include "ClaimableSquareGameGrid.h"
#include "GridClaimingActor.h"


bool AClaimableSquareGameGrid::AddGridActor(AGridClaimingActor * InActor, FGridTile TileLocation, bool TickOnAdd)
{
	if (!InActor) return false;

	TArray<FGridTileOffset> attemptedclaimspace = InActor->GetRelativeClaimSpace();
	TArray<FGridTile> gridtiles;

	if (GetGridTilesFromOffset(InActor->GetRootGridTile(), attemptedclaimspace, gridtiles, true))
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
	

	return false;
}
