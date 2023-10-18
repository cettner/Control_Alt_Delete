// Fill out your copyright notice in the Description page of Project Settings.


#include "GridTile.h"
#include "GameGrid.h"


AGameGrid* UGridTile::GetGameGrid() const
{
	return Cast<AGameGrid>(GetOuter());
}

int32 UGridTile::GetTileID() const
{
	return TileID;
}

bool UGridTile::IsTileValid() const
{
	return TileID != INVALID_TILE_ID;
}

const TArray<FGridTileNeighbor> UGridTile::GetNeighbors() const
{
	return Neighbors;
}

void UGridTile::SetTileFillColor(const FLinearColor InColor)
{
	GetGameGrid()->SetTileColor(GetTileID(), InColor);
}

void UGridTile::SetTileVisible(const bool InIsVisible)
{
	GetGameGrid()->SetTileVisible(GetTileID(), InIsVisible);
}

void UGridTile::SetupTile(const int32 InID, const FVector InTileCenter)
{
	TileID = InID;
	TileCenter = InTileCenter;

	if (IsTileValid())
	{
		const AGameGrid* grid = GetGameGrid();
		const float edgelength = grid->GetTileEdgeLength();
		const FVector tilecenter = GetTileCenter();

		const float gridheight = tilecenter.Z;
		const float offset = edgelength * .5f;
		const FVector p1 = tilecenter + FVector(-offset, offset, gridheight);
		const FVector p2 = tilecenter + FVector(offset, offset, gridheight);
		const FVector p3 = tilecenter + FVector(-offset, -offset, gridheight);
		const FVector p4 = tilecenter + FVector(offset, -offset, gridheight);

		TileBounds.Add(FLine(p1, p2));
		TileBounds.Add(FLine(p2, p4));
		TileBounds.Add(FLine(p4, p3));
		TileBounds.Add(FLine(p3, p1));

		/*Calculate vectors to compute the normal*/
		const FVector v1 = p2 - tilecenter;
		const FVector v2 = p4 - tilecenter;
		TileNormal = v2.Cross(v1);
		TileNormal.Normalize();

	}
}

void UGridTile::AddNeighbor(const FGridTileNeighbor& InNeighbor)
{
	Neighbors.AddUnique(InNeighbor);
}

TSet<FLine> UGridTile::GetTileBoundaryLines() const
{
	return TileBounds;
}

UWorld* UGridTile::GetWorld() const
{
	UWorld* retval = nullptr;
	AActor* actor = Cast<AActor>(GetOuter());

	if (IsValid(actor))
	{
		retval = actor->GetWorld();
	}

	return retval;
}

void UGridTile::DrawDebugData()
{
	DrawDebugString(GetWorld(), TileCenter, FString::FromInt(TileID), GetGameGrid(), FColor::Red);
}

uint32 GetTypeHash(const FLine& Thing)
{
	const uint32 Hash = FCrc::MemCrc32(&Thing, sizeof(FLine));
	return Hash;
}
