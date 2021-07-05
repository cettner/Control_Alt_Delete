// Fill out your copyright notice in the Description page of Project Settings.


#include "ClaimModifierType.h"
#include "ClaimableSquareGameGrid.h"


bool UClaimModifierType::IsOverlapping() const
{
	bool retval = false;
	if (WorkingGrid != nullptr)
	{
		for (int i = 0; i < AppliedTiles.Num(); i++)
		{
			TArray<UGridModifierType* > mods = WorkingGrid->GetActiveModifiers(AppliedTiles[i]);
			if (mods.Num() > 1)
			{
				retval = true;
				break;
			}
		}
	}
	return retval;
}

void UClaimModifierType::OnModifierApply(AClaimableSquareGameGrid * ParentGrid, FGridTile TileLocation, AGridClaimingActor * Invoker)
{
	ParentGrid->SetSelectedTile(TileLocation);
}


void UClaimModifierType::OnModiferRemoved(AClaimableSquareGameGrid * ParentGrid, FGridTile TileLocation, AGridClaimingActor * Invoker)
{
	TArray<UGridModifierType*> othermods = ParentGrid->GetActiveModifiers(TileLocation);
	if (othermods.Num() == 0)
	{
		ParentGrid->HideSelectedTile(TileLocation);
	}
}
