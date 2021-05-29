// Fill out your copyright notice in the Description page of Project Settings.


#include "GridClaimingActor.h"
#include "ClaimableSquareGameGrid.h"

void AGridClaimingActor::InitializeClaimSpace()
{

}

TArray<TSubclassOf<UGridModifierType>> AGridClaimingActor::GetActiveModifiers(FGridTile TileData)
{
	return(Modifiers);
}

ASquareGameGrid * AGridClaimingActor::AttachToGrid(FVector StartLocation, ASquareGameGrid * InGrid)
{
	ASquareGameGrid * foundgrid = Super::AttachToGrid(StartLocation, InGrid);

	AClaimableSquareGameGrid * claimgrid = Cast<AClaimableSquareGameGrid>(foundgrid);

	if (claimgrid)
	{
		claimgrid->AddGridActor(this, GetRootGridTile());
	}


	return(foundgrid);
}

bool AGridClaimingActor::SetTileLocation(FGridTile Moveto)
{
	return false;
}

TArray<FGridTileOffset> AGridClaimingActor::GetRelativeClaimSpace() const
{
	return ClaimSpace;
}

void AGridClaimingActor::SetGridClaimSpace(TArray<FGridTile> ClaimedTiles, ASquareGameGrid * OwningGrid)
{

}

TArray<FGridTile> AGridClaimingActor::GetGridClaimSpace() const
{
	return TArray<FGridTile>();
}
