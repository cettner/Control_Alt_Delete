// Fill out your copyright notice in the Description page of Project Settings.


#include "ClaimSpaceLayer.h"
#include "../GameGrid.h"



UClaimSpaceLayer::UClaimSpaceLayer()
{
	bIsLayerVisible = true;
}

bool UClaimSpaceLayer::IsClaimValid() const
{
	const AGameGrid* grid = GetGameGrid();
	bool retval = (ActiveTiles.Num() > 0) && (grid != nullptr);
	
	if (retval)
	{
		TArray<UClaimSpaceLayer *> claimspaces = grid->GetLayersOfClass<UClaimSpaceLayer>();
		for (int i = 0; i < claimspaces.Num(); i++)
		{
			const bool bshouldcheck = IsValid(claimspaces[i]) && (claimspaces[i] != this);
			if (bshouldcheck)
			{
				for (UGridTile* activetile : ActiveTileSet)
				{
					if (claimspaces[i]->ContainsTile(activetile))
					{
						retval = false;
						return retval;
					}
				}
			}
		}
	}

	return retval;
}

void UClaimSpaceLayer::ShowTile(UGridTile* InTile)
{
	InTile->SetTileFillColor(FLinearColor::Blue);
	InTile->SetTileVisible(true);
}

void UClaimSpaceLayer::HideTile(UGridTile* InTile)
{
	InTile->SetTileVisible(false);
}

