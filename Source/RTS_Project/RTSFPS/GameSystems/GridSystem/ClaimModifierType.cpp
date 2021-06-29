// Fill out your copyright notice in the Description page of Project Settings.


#include "ClaimModifierType.h"
#include "ClaimableSquareGameGrid.h"


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
