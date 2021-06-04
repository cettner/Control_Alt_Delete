// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GridAttatchmentActor.h"
#include "SquareGameGrid.h"
#include "GridModifierType.h"

#include "Components/BoxComponent.h"
#include "GridClaimingActor.generated.h"


USTRUCT()
struct FGridTileOffset
{
	GENERATED_USTRUCT_BODY()

	int RowOffset = 0;
	int ColOffset = 0;

	friend bool operator == (const FGridTileOffset& Myself, const FGridTileOffset& Other)
	{
		bool isSame = Myself.RowOffset == Other.RowOffset;
		isSame &= Myself.ColOffset == Other.ColOffset;

		return(isSame);
	}
};

UCLASS()
class RTS_PROJECT_API AGridClaimingActor : public AGridAttatchmentActor
{
	GENERATED_BODY()

public:
	AGridClaimingActor();


protected:
	virtual void OnConstruction(const FTransform & Transform) override;

protected:
	virtual void InitializeClaimSpace(ASquareGameGrid * InGrid);
	virtual void PreTileChange(FGridTile NewTile);
	virtual void PostTileChange(FGridTile NewTile, FGridTile PrevTile = FGridTile());
	

protected:
	virtual ASquareGameGrid * AttachToGrid(FVector StartLocation, ASquareGameGrid * InGrid = nullptr) override;
	virtual bool SetTileLocation(FGridTile Moveto) override;

public:
	virtual TArray<TSubclassOf<UGridModifierType>> GetActiveModifiers(FGridTile TileData);
	virtual TArray<FGridTileOffset> GetRelativeClaimSpace() const;
	virtual void SetGridClaimSpace(TArray<FGridTile> ClaimedTiles, ASquareGameGrid * OwningGrid);
	virtual TArray<FGridTile> GetGridClaimSpace() const;

protected:
	UPROPERTY(EditDefaultsOnly, Category = Modifiers)
	UBoxComponent * ClaimSpaceComp;

protected:
    TArray<FGridTile> GridClaimSpace;
	TArray<FGridTileOffset> RelativeClaimSpace;
	TArray<TSubclassOf<UGridModifierType>> Modifiers;
};
