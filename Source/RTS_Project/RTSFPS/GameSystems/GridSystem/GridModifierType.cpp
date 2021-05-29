// Fill out your copyright notice in the Description page of Project Settings.


#include "GridModifierType.h"
#include "GridClaimingActor.h"
#include "ClaimableSquareGameGrid.h"


FLinearColor UGridModifierType::GetTileColor() const
{
	return FLinearColor();
}

bool UGridModifierType::IsModifierActive() const
{
	return bIsActive;
}

void UGridModifierType::ApplyModifier(AClaimableSquareGameGrid * ParentGrid, FGridTile TileLocation, AGridClaimingActor * Invoker) const
{
}
