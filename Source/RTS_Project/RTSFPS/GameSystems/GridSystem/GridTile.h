// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GridTile.generated.h"



constexpr int32 INVALID_TILE_ID = 0xFFFFFFFF;

USTRUCT()
struct FGridTileNeighbor
{
    GENERATED_USTRUCT_BODY()

public:
    FGridTileNeighbor()
    {

    }

    FGridTileNeighbor(UGridTile* InNeighborTile, bool InIsCornerNeighbor = false)
    {
        NeighborTile = InNeighborTile;
        bIsCornerNeighbor = InIsCornerNeighbor;
    }

    friend bool operator == (const FGridTileNeighbor& Myself, const FGridTileNeighbor& Other)
    {
        const bool retval = (Myself.NeighborTile == Other.NeighborTile) && (Myself.bIsCornerNeighbor == Other.bIsCornerNeighbor);
        return retval;
    }

    bool bIsCornerNeighbor = false;
    UGridTile* NeighborTile = nullptr;
};

USTRUCT()
struct FLine
{
    GENERATED_USTRUCT_BODY()
public:
    FLine()
    {
        StartPoint = FVector();
        EndPoint = FVector();
    }

    FLine(const FVector &InStart, const FVector &InEnd) 
    {
        StartPoint = InStart;
        EndPoint = InEnd;
    }

    friend bool operator == (const FLine& Myself, const FLine& Other)
    {
        const bool bstartpointsame = (Myself.StartPoint == Other.StartPoint);
        const bool bendpointsame = (Myself.EndPoint == Other.EndPoint);
        return bstartpointsame && bendpointsame;
    }

    FVector StartPoint;
    FVector EndPoint;


};

uint32 GetTypeHash(const FLine& Thing);


UCLASS()
class RTS_PROJECT_API UGridTile : public UObject
{
    friend class AGameGrid;

	GENERATED_BODY()

public:
    int32 GetTileID() const;
    AGameGrid* GetGameGrid() const;
    template<class T>
    T* GetGameGrid() const { Cast<T>(GetGameGrid()); }
    bool IsTileValid() const;
    FORCEINLINE FVector GetTileCenter() const { return TileCenter; }
    FORCEINLINE FVector GetTileNormal() const { return TileNormal; }
    const TArray<FGridTileNeighbor>& GetNeighbors() const;
    void SetTileFillColor(const FLinearColor InColor);
    void SetTileVisible(const bool InIsVisible);

protected: 
    void SetupTile(const int32 InID, const FVector InTileCenter);
    void AddNeighbor(const FGridTileNeighbor& InNeighbor);
    const TSet<FLine>& GetTileBoundaryLines() const;
    UWorld* GetWorld() const;

protected:
    virtual void DrawDebugData();

protected:
    int32 TileID = INVALID_TILE_ID;

    FVector TileCenter = FVector();

    FVector TileNormal = FVector();

    TArray<FGridTileNeighbor> Neighbors = TArray<FGridTileNeighbor>();

    TSet<FLine> TileBounds = TSet<FLine>();

};
