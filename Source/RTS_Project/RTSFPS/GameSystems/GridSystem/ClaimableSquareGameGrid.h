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

public:
	  virtual bool AddGridActor(AGridClaimingActor * InActor, FGridTile TileLocation, bool TickOnAdd = true);
	  virtual bool RemoveGridActor(AGridClaimingActor * InActor, bool TickOnRemoval = false);
	  virtual bool MoveGridActor(AGridClaimingActor * InActor, FGridTile TileLocation);
	  virtual bool CanMoveTo(AGridClaimingActor * InActor, FGridTile TileLocation);
	  virtual TArray<TSubclassOf<UGridModifierType>> GetActiveModifiers(FGridTile TileLocation) const;
	  virtual bool ApplyModifier(FGridTile TileLocation, TSubclassOf<UGridModifierType>, AGridClaimingActor * Source);
	  virtual bool RemoveModifier(FGridTile TileLocation, TSubclassOf<UGridModifierType>, AGridClaimingActor * Source);

protected:
	virtual bool GetGridTilesFromOffset(FGridTile StartTile, TArray<FGridTileOffset> Offsets,  TArray<FGridTile> & OutTiles, bool bisstartinclusive = false) const;

protected:
	TArray<AGridClaimingActor *> GridActors;
	TMultiMap<uint32_t, AGridClaimingActor *> ModifierMap;
};
