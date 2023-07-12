// Fill out your copyright notice in the Description page of Project Settings.


#include "GridLayer.h"
#include "GameGrid.h"

void UGridLayer::LayerInitialize(AGameGrid* InGrid, const TArray<UGridTile*>& InActiveTiles, AActor* InApplicator)
{
	ParentGrid = InGrid;
	ActiveTiles = TArray<UGridTile*>(InActiveTiles);
	for (int i = 0; i < InActiveTiles.Num(); i++)
	{
		ActiveTileSet.Add(InActiveTiles[i]);
	}

	Applicator = InApplicator;
}

void UGridLayer::OnLayerActivate()
{
	for (int i = 0; i < ActiveTiles.Num(); i++)
	{
		PostActivateTile(ActiveTiles[i]);
	}

	bIsLayerActivated = true;
}

uint32 UGridLayer::OnLayerDeactivate()
{
	uint32 retval = 0U;

	for (int32 i = ActiveTiles.Num() - 1; i > INDEX_NONE; i--)
	{
		RemoveTile(ActiveTiles[i]);
		retval++;
	}
	bIsLayerActivated = false;
	return retval;
}

void UGridLayer::OnShowLayer()
{
	for (int i = 0; i < ActiveTiles.Num(); i++)
	{
		ShowTile(ActiveTiles[i]);
	}
}

void UGridLayer::OnHideLayer()
{
	for (int i = 0; i < ActiveTiles.Num(); i++)
	{
		HideTile(ActiveTiles[i]);
	}
}

void UGridLayer::PostActivateTile(UGridTile* InTile)
{
	if (IsLayerVisible())
	{
		ShowTile(InTile);
	}
}

void UGridLayer::PostDeactivateTile(UGridTile* InTile)
{
	if (IsLayerVisible())
	{
		HideTile(InTile);
	}
}

void UGridLayer::ShowTile(UGridTile* InTile)
{
}

void UGridLayer::HideTile(UGridTile* InTile)
{
}

void UGridLayer::SetLayerVisibility(bool InbIsVisible)
{
	bIsLayerVisible = InbIsVisible;

	if (bIsLayerVisible == true)
	{
		OnShowLayer();
	}
	else
	{
		OnHideLayer();
	}
}

void UGridLayer::AddTile(UGridTile* InTile)
{
	bool balreadyinset = false;
	ActiveTileSet.Add(InTile, &balreadyinset);

	if(balreadyinset)
	{
		UE_LOG(LogTemp, Warning, TEXT("Warning: Duplicate GridTile Added to LayerSet of %s"), *GetName())
	}
	else
	{
		ActiveTiles.Emplace(InTile);
		if (bIsLayerActivated)
		{
			PostActivateTile(InTile);
		}
	}


}

bool UGridLayer::RemoveTile(UGridTile* InTile)
{
	const int32 removesuccess = ActiveTileSet.Remove(InTile);

	if (removesuccess > (int32)0)
	{
		ActiveTiles.Remove(InTile);
		PostDeactivateTile(InTile);
	}
	const bool retval = removesuccess > (int32)0;

	return retval;
}

bool UGridLayer::ContainsTile(const UGridTile* InTile) const
{
	return  (InTile != nullptr) && (ActiveTileSet.Contains(InTile));
}

int UGridLayer::ResetTiles(const TSet<UGridTile*>& InActiveTiles, const bool& InbShouldActivateSharedTiles)
{
	/*return the number of Activated tiles*/
	int retval = 0;
	/*We shut down all tiles then Reactivate them with the new set*/
	if (InbShouldActivateSharedTiles)
	{
		OnLayerDeactivate();
		ActiveTiles = TArray<UGridTile*>(InActiveTiles.Array());
		ActiveTileSet = TSet<UGridTile*>(InActiveTiles);
		OnLayerActivate();
		retval = ActiveTiles.Num();
	}
	else
	{
		/*Only Activate New Tiles and Deactivate Removed Ones*/

		/*Store Shared Tiles for speed*/
		TSet<UGridTile*> sharedtiles = TSet<UGridTile*>();		
		for (int i = ActiveTiles.Num() - 1; i > INDEX_NONE; i--)
		{
			/*Remove Tile that's not in the active set*/
			if (!InActiveTiles.Contains(ActiveTiles[i]))
			{
				RemoveTile(ActiveTiles[i]);
			}
			else
			{
				sharedtiles.Add(ActiveTiles[i]);
			}
		}

		/*Find and Add New Tiles*/
		for (UGridTile* tile : InActiveTiles)
		{
			if (!sharedtiles.Contains(tile))
			{
				AddTile(tile);
				retval++;
			}
		}
	}

	return retval;
}

TArray<UGridTile*> UGridLayer::GetDefaultTileSet(const AGameGrid* InGrid) const
{
	TArray<UGridTile*> retval = TArray<UGridTile*>();

	if (IsValid(InGrid))
	{
		retval = InGrid->GetTiles();
	}

	return retval;
}
