// Fill out your copyright notice in the Description page of Project Settings.


#include "SuperTileManager.h"
#include "..\GameGrid.h"

USuperTileManager::USuperTileManager()
{
    bIsLayerVisible = true;
}

void USuperTileManager::InitializeTileNeighbors(TArray<UGridLayer*>& InLayers)
{
    const AGameGrid* grid = GetGameGrid();
    int numsuperrows = grid->GetMaxRows() / ColomnSize;
    int numsupercolomns = grid->GetMaxCols() / RowSize;
    const bool haspartialrow = (grid->GetMaxRows() % ColomnSize) > 0;
    const bool haspartialcolomn = (grid->GetMaxCols() % RowSize) > 0;

    if (haspartialrow)
    {
        numsuperrows++;
    }
    if (haspartialcolomn)
    {
        numsupercolomns++;
    }

    // Loop through all the super tiles and assign their neighbors
    for (int32 i = 0; i < InLayers.Num(); i++)
    {
        FSuperTile supertile = FSuperTile();
        supertile.AppliedLayer = InLayers[i];

        // Get the row and column indices of the super tile
        const int32 rowindex = i / numsupercolomns;
        const int32 colindex = i % numsupercolomns;

        // Top Neighbor
        if (rowindex < (numsuperrows - 1))
        {
            supertile.TileNeighbors.Emplace(InLayers[i + numsupercolomns]);
        }
        // Check bottom neighbor
        if (rowindex > 0)
        {
            supertile.TileNeighbors.Emplace(InLayers[i - numsupercolomns]);
        }
        // Check left neighbor
        if (colindex > 0)
        {
            supertile.TileNeighbors.Emplace(InLayers[i - 1]);
        }
        // Check right neighbor
        if (colindex < numsupercolomns - 1)
        {
            supertile.TileNeighbors.Emplace(InLayers[i + 1]);
        }


        SuperTiles.Add(supertile);
    }
}

FLinearColor USuperTileManager::GetTileHashColor(const int& InHashID)
{
    const int32 Hue = InHashID % 360;
    const int32 Saturation = 150 + (InHashID * 31) % 101; // Keep it in the range of 150-250
    const int32 Value = 150 + ((InHashID * 17) % 51); // Keep it in the range of 150-200
    const FLinearColor retval = FLinearColor::MakeFromHSV8(Hue, Saturation, Value);

    return retval;
}

void USuperTileManager::LayerInitialize(AGameGrid* InGrid, const TArray<UGridTile*>& InActiveTiles, AActor* InApplicator)
{
    Super::LayerInitialize(InGrid, InActiveTiles, InApplicator);

    if (LayerClass != nullptr)
    {
        TArray<UGridLayer*> superlayers = TArray<UGridLayer*>();

        // Determine the number of full and partial super tiles in each dimension
        const int32 numrows = InGrid->GetMaxRows();
        const int32 numcols = InGrid->GetMaxCols();
        const int32 rowremainder = numrows % RowSize;
        const int32 colremainder = numcols % ColomnSize;
        const int32 numfullrows = numrows / RowSize;
        const int32 numfullcols = numcols / ColomnSize;
        const int32 numpartialrows = rowremainder > 0 ? 1 : 0;
        const int32 numpartialcols = colremainder > 0 ? 1 : 0;

        // Loop through all the super tiles and create them, handling partial super tiles as needed
        for (int32 rowindex = 0; rowindex < numfullrows + numpartialrows; rowindex++)
        {
            const int32 numrowsintile = (rowindex < numfullrows) ? RowSize : rowremainder;
            for (int32 colindex = 0; colindex < numfullcols + numpartialcols; colindex++)
            {
                const int32 numcolsintile = (colindex < numfullcols) ? ColomnSize : colremainder;
                TSet<UGridTile*> subsettiles;

                // Add all the tiles within the current super tile
                for (int32 subrowindex = 0; subrowindex < numrowsintile; subrowindex++)
                {
                    for (int32 subcolindex = 0; subcolindex < numcolsintile; subcolindex++)
                    {
                        const int32 globalrowindex = rowindex * RowSize + subrowindex;
                        const int32 globalcolindex = colindex * ColomnSize + subcolindex;
                        const int32 tileindex = globalrowindex * numcols + globalcolindex;
                        if (tileindex < InActiveTiles.Num())
                        {
                            subsettiles.Add(InActiveTiles[tileindex]);
                        }
                    }
                }

                // Create the super tile
                UGridLayer* newsupertile = InGrid->AddGridLayer(LayerClass, subsettiles.Array(), InApplicator);
                checkf(newsupertile, TEXT("USuperTileManager::LayerInitialize Failed to create supertile"));
                superlayers.Emplace(newsupertile);
            }
        }

       InitializeTileNeighbors(superlayers);
    }
}

void USuperTileManager::OnShowLayer()
{
    for (int32 i = 0; i < SuperTiles.Num(); i++)
    {
       const TSet<UGridTile*> tiles = SuperTiles[i]->GetTiles();
       const int32 layerid = SuperTiles[i]->GetLayerID();
       const FLinearColor supertilecolor = GetTileHashColor(layerid);
       for (UGridTile * tile : tiles)
       {
           tile->SetTileFillColor(supertilecolor);
           tile->SetTileVisible(true);
       }
    }
}
