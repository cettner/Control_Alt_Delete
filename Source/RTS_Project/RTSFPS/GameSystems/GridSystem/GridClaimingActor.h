// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridAttatchmentActor.h"
#include "SquareGameGrid.h"
#include "GridModifierType.h"
#include "GridClaimingActor.generated.h"


USTRUCT()
struct FGridTileOffset
{
	GENERATED_USTRUCT_BODY()

	int RowOffset = 0;
	int ColOffset = 0;
};

UCLASS()
class RTS_PROJECT_API AGridClaimingActor : public AGridAttatchmentActor
{
	GENERATED_BODY()

protected:
	virtual void InitializeClaimSpace(ASquareGameGrid * InGrid);
	virtual void OnTileChange(FGridTile NewTile);

protected:
	virtual ASquareGameGrid * AttachToGrid(FVector StartLocation, ASquareGameGrid * InGrid = nullptr) override;
	virtual bool SetTileLocation(FGridTile Moveto) override;

public:
	virtual TArray<TSubclassOf<UGridModifierType>> GetActiveModifiers(FGridTile TileData);
	virtual TArray<FGridTileOffset> GetRelativeClaimSpace() const;
	virtual void SetGridClaimSpace(TArray<FGridTile> ClaimedTiles, ASquareGameGrid * OwningGrid);
	virtual TArray<FGridTile> GetGridClaimSpace() const;


protected:
    TArray<FGridTile> GridClaimSpace;
	TArray<FGridTileOffset> RelativeClaimSpace;
	TArray<TSubclassOf<UGridModifierType>> Modifiers;
};
