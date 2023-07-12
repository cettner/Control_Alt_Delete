// Fill out your copyright notice in the Description page of Project Settings.


#include "GridLayerComponent.h"
#include "GridAttachmentActor.h"

// Sets default values for this component's properties
UGridLayerComponent::UGridLayerComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
    bWantsInitializeComponent = true;
}

TSet<UGridTile*> UGridLayerComponent::GetGridSpace() const
{
    TSet<UGridTile*> Result;
    AGameGrid* GameGrid = GetGameGrid();

    if (GameGrid != nullptr)
    {
        const FVector Origin = GetComponentLocation();
        const FVector Extent = GetScaledBoxExtent();
        const float TileSize = GameGrid->GetTileEdgeLength();
        const FVector BoxMin = FVector(FMath::FloorToInt((Origin - Extent).X / TileSize),
            FMath::FloorToInt((Origin - Extent).Y / TileSize),
            FMath::FloorToInt((Origin - Extent).Z / TileSize)) * TileSize;
        const FVector BoxMax = FVector(FMath::CeilToInt((Origin + Extent).X / TileSize),
            FMath::CeilToInt((Origin + Extent).Y / TileSize),
            FMath::CeilToInt((Origin + Extent).Z / TileSize)) * TileSize;

        const FVector2D TileSteps = FVector2D(TileSize, TileSize);
        const FVector2D TileStart = FVector2D(BoxMin.X, BoxMin.Y);
        const FVector2D TileEnd = FVector2D(BoxMax.X, BoxMax.Y);

        const FBox BoundsBox = FBox(BoxMin, BoxMax);

        for (FVector2D TileCoords = TileStart; TileCoords.X <= TileEnd.X; TileCoords.X += TileSteps.X)
        {
            for (TileCoords.Y = TileStart.Y; TileCoords.Y <= TileEnd.Y; TileCoords.Y += TileSteps.Y)
            {
                FVector TileLocation = FVector(TileCoords, Origin.Z);
                UGridTile* Tile = GameGrid->GetTileFromLocation(TileLocation);
                if (Tile != nullptr && BoundsBox.IsInside(Tile->GetTileCenter()))
                {
                    Result.Add(Tile);
                }
            }
        }
    }

    return Result;
}

void UGridLayerComponent::ApplyLayers()
{
	AGameGrid * grid = GetGameGrid();
    if (grid != nullptr)
    {
        TSet<UGridTile*> gridspace = GetGridSpace();
        AActor* applicator = GetAttachParentActor();

        for (int i = 0; i < LayerClasses.Num(); i++)
        {
            if (LayerClasses[i] != nullptr)
            {
                UGridLayer* layer = grid->AddGridLayer(LayerClasses[i], gridspace.Array(), applicator);
                ActiveLayers.Emplace(layer);
            }
        }
    }
}

AGameGrid* UGridLayerComponent::GetGameGrid() const
{
	AGameGrid* retval = nullptr;
	if (const AGridAttachmentActor * parent = Cast<AGridAttachmentActor>(GetAttachParentActor()))
	{
		retval = parent->GetGameGrid();
	}

	return retval;
}

void UGridLayerComponent::InitializeComponent()
{
	Super::InitializeComponent();
    //ApplyLayers();

}

void UGridLayerComponent::OnRegister()
{
    Super::OnRegister();
}

void UGridLayerComponent::PostLoad()
{
    Super::PostLoad();
    //ApplyLayers();
}

void UGridLayerComponent::OnComponentCreated()
{
    Super::OnComponentCreated();
}

void UGridLayerComponent::Activate(bool bReset)
{
    Super::Activate();
}

void UGridLayerComponent::PostRootTileChanged()
{
	const TSet<UGridTile*> gridspace = GetGridSpace();
	for (int i = 0; i < ActiveLayers.Num(); i++)
	{
		ActiveLayers[i]->ResetTiles(gridspace);
	}
}

void UGridLayerComponent::PostEditComponentMove(bool bFinished)
{
	Super::PostEditComponentMove(bFinished);

	if (bFinished == true)
	{
		GetComponentLocation();

	}
}

void UGridLayerComponent::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
}


