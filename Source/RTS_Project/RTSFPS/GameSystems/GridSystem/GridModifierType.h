// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SquareGameGrid.h"
#include "GridModifierType.generated.h"




class AClaimableSquareGameGrid;
class AGridClaimingActor;

UCLASS()
class RTS_PROJECT_API UGridModifierType : public UObject
{
	GENERATED_BODY()

  public:
	virtual bool IsModifierActive(FGridTile TileLocation) const;
	void ApplyModifier(AClaimableSquareGameGrid * ParentGrid, FGridTile TileLocation, AGridClaimingActor * Invoker);
	void ApplyModifier(AClaimableSquareGameGrid * ParentGrid, TArray<FGridTile> TileLocations, AGridClaimingActor * Invoker);
	bool RemoveModifier(AClaimableSquareGameGrid * ParentGrid, FGridTile TileLocation, AGridClaimingActor * Invoker);
	bool RemoveModifier(AClaimableSquareGameGrid * ParentGrid, TArray<FGridTile> TileLocations, AGridClaimingActor * Invoker);
	virtual void RemoveAll(AClaimableSquareGameGrid * ParentGrid, AGridClaimingActor * Invoker);
  
  protected:
	  virtual void OnModifierApply(AClaimableSquareGameGrid * ParentGrid, FGridTile TileLocation, AGridClaimingActor * Invoker);
	  virtual void OnModiferRemoved(AClaimableSquareGameGrid * ParentGrid, FGridTile TileLocation, AGridClaimingActor * Invoker);

  protected:
	bool bIsActive = true;
	bool bCallsRemovalOnDestruction = true;

	AClaimableSquareGameGrid * WorkingGrid = nullptr;
	TArray<FGridTile> AppliedTiles = TArray<FGridTile>();

};
