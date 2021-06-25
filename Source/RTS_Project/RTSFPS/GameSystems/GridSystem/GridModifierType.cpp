// Fill out your copyright notice in the Description page of Project Settings.


#include "GridModifierType.h"
#include "GridClaimingActor.h"
#include "ClaimableSquareGameGrid.h"





bool UGridModifierType::IsModifierActive(FGridTile TileLocation) const
{
	return AppliedTiles.Contains(TileLocation);
}

void UGridModifierType::ApplyModifier(AClaimableSquareGameGrid * ParentGrid, FGridTile TileLocation, AGridClaimingActor * Invoker)
{
	if (TileLocation.IsValid && !AppliedTiles.Contains(TileLocation))
	{
		AppliedTiles.Emplace(TileLocation);
		WorkingGrid = ParentGrid;
		ParentGrid->SetSelectedTile(TileLocation);
	}
}

void UGridModifierType::ApplyModifier(AClaimableSquareGameGrid * ParentGrid, TArray<FGridTile> TileLocations, AGridClaimingActor * Invoker)
{
	for (int i = 0; i < TileLocations.Num(); i++)
	{
		ApplyModifier(ParentGrid, TileLocations[i], Invoker);
	}
}

bool UGridModifierType::OnModifierRemoved(AClaimableSquareGameGrid * ParentGrid, FGridTile TileLocation, AGridClaimingActor * Invoker)
{
	bool retval = false;
	int32 index = AppliedTiles.IndexOfByKey(TileLocation);
	if (index > INDEX_NONE)
	{
		AppliedTiles.RemoveAt(index);
		TArray<UGridModifierType*> othermods = ParentGrid->GetActiveModifiers(TileLocation);
		if (othermods.Num() == 0)
		{
			ParentGrid->HideSelectedTile(TileLocation);
		}
	}

	retval = index > INDEX_NONE;
	return(retval);
}

bool UGridModifierType::OnModifierRemoved(AClaimableSquareGameGrid * ParentGrid, TArray<FGridTile> TileLocations, AGridClaimingActor * Invoker)
{
	bool retval = true;
	for (int i = 0; i < TileLocations.Num(); i++)
	{
		retval &= OnModifierRemoved(ParentGrid, TileLocations[i], Invoker);
	}

	return(retval);
}

void UGridModifierType::RemoveAll(AClaimableSquareGameGrid * ParentGrid, AGridClaimingActor * Invoker)
{
		OnModifierRemoved(ParentGrid, AppliedTiles, Invoker);
}

