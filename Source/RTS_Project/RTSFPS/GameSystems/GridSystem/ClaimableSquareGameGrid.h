// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SquareGameGrid.h"
#include "GridModifierType.h"
#include "ClaimableSquareGameGrid.generated.h"


/*Forward Declaration*/
class AGridClaimingActor;
struct FGridTileOffset;

UCLASS()
class RTS_PROJECT_API AClaimableSquareGameGrid : public ASquareGameGrid
{
	GENERATED_BODY()

protected:
	  virtual void OnConstruction(const FTransform & Transform) override;

public:
	  virtual bool AddGridActor(AGridClaimingActor * InActor, FGridTile TileLocation, bool TickOnAdd = true);
	  virtual bool RemoveGridActor(AGridClaimingActor * InActor, bool TickOnRemoval = false);
	  virtual bool MoveGridActor(AGridClaimingActor * InActor, FGridTile TileLocation);
	  virtual bool CanMoveTo(AGridClaimingActor * InActor, FGridTile TileLocation);
	  
	  virtual TArray<UGridModifierType *> GetActiveModifiers(FGridTile TileLocation) const;
	  
	  virtual bool ApplyModifier(FGridTile TileLocation, UGridModifierType * Modifier, AGridClaimingActor * Source);
	  virtual bool ApplyModifier(TArray<FGridTile> TileLocations, UGridModifierType * Modifier, AGridClaimingActor * Source);
	  virtual bool RemoveModifier(FGridTile TileLocation, UGridModifierType * Modifier, AGridClaimingActor * Source);

	  virtual void SimulateGrid();

protected:
	virtual bool GetGridTilesFromOffset(FGridTile StartTile, TArray<FGridTileOffset> Offsets,  TArray<FGridTile> & OutTiles, bool bisstartinclusive = false) const;

public:
	bool ISSimulatingEffects() const;

protected:
	TArray<AGridClaimingActor *> GridActors;
};
