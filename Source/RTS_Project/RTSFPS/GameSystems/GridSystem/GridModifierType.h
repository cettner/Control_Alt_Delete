// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GridModifierType.generated.h"




class AClaimableSquareGameGrid;
class AGridClaimingActor;
struct FGridTile;

UCLASS()
class RTS_PROJECT_API UGridModifierType : public UObject
{
	GENERATED_BODY()

  public:
	virtual bool IsModifierActive(FGridTile TileLocation) const;
	virtual void ApplyModifier(AClaimableSquareGameGrid * ParentGrid, FGridTile TileLocation, AGridClaimingActor * Invoker);
	virtual void ApplyModifier(AClaimableSquareGameGrid * ParentGrid, TArray<FGridTile> TileLocations, AGridClaimingActor * Invoker);
	virtual bool OnModifierRemoved(AClaimableSquareGameGrid * ParentGrid, FGridTile TileLocation, AGridClaimingActor * Invoker);
	virtual bool OnModifierRemoved(AClaimableSquareGameGrid * ParentGrid, TArray<FGridTile> TileLocations, AGridClaimingActor * Invoker);
	virtual void RemoveAll(AClaimableSquareGameGrid * ParentGrid, AGridClaimingActor * Invoker);

	
  protected:
	bool bIsActive = true;
	bool bCallsRemovalOnDestruction = true;

	AClaimableSquareGameGrid * WorkingGrid = nullptr;
	TArray<FGridTile> AppliedTiles = TArray<FGridTile>();

};
