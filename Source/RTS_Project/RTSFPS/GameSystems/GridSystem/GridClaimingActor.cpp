// Fill out your copyright notice in the Description page of Project Settings.


#include "GridClaimingActor.h"
#include "ClaimableSquareGameGrid.h"

void AGridClaimingActor::InitializeClaimSpace(ASquareGameGrid * InGrid)
{

}

TArray<TSubclassOf<UGridModifierType>> AGridClaimingActor::GetActiveModifiers(FGridTile TileData)
{
	return(Modifiers);
}

ASquareGameGrid * AGridClaimingActor::AttachToGrid(FVector StartLocation, ASquareGameGrid * InGrid)
{
	ASquareGameGrid * foundgrid = Super::AttachToGrid(StartLocation, InGrid);
	InitializeClaimSpace(foundgrid);

	AClaimableSquareGameGrid * claimgrid = Cast<AClaimableSquareGameGrid>(foundgrid);

	if (claimgrid && claimgrid->AddGridActor(this, GetRootGridTile()))
	{
		OnTileChange(GetRootGridTile());
	}


	return(foundgrid);
}

bool AGridClaimingActor::SetTileLocation(FGridTile Moveto)
{
	return false;
}

TArray<FGridTileOffset> AGridClaimingActor::GetRelativeClaimSpace() const
{
	return RelativeClaimSpace;
}

void AGridClaimingActor::SetGridClaimSpace(TArray<FGridTile> ClaimedTiles, ASquareGameGrid * OwningGrid)
{
	GridClaimSpace = ClaimedTiles;
}

TArray<FGridTile> AGridClaimingActor::GetGridClaimSpace() const
{
	return GridClaimSpace;
}
