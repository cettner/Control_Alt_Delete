// Fill out your copyright notice in the Description page of Project Settings.


#include "GridModifierType.h"
#include "GridClaimingActor.h"
#include "ClaimableSquareGameGrid.h"


FLinearColor UGridModifierType::GetTileColor() const
{
	return FLinearColor::Red;
}

bool UGridModifierType::IsModifierActive() const
{
	return bIsActive;
}

void UGridModifierType::ApplyModifier(AClaimableSquareGameGrid * ParentGrid, FGridTile TileLocation, AGridClaimingActor * Invoker) const
{
	TArray<FGridTile> TileArray = TArray<FGridTile>(&TileLocation,1);
	ParentGrid->SetSelectedTiles(TileArray,GetTileColor());
}

void UGridModifierType::OnModifierRemoved(AClaimableSquareGameGrid * ParentGrid, FGridTile TileLocation, AGridClaimingActor * Invoker) const
{
	TArray<FGridTile> TileArray = TArray<FGridTile>(&TileLocation, 1);
	ParentGrid->HideSelectedTiles(TileArray);
}
