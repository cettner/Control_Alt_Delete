// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SquareGameGrid.h"
#include "GridAttatchmentActor.generated.h"








UCLASS()
class RTS_PROJECT_API AGridAttatchmentActor : public AActor
{
	GENERATED_BODY()
	

public:	
	// Sets default values for this actor's properties
	AGridAttatchmentActor();

public:
	
	/*Attmpts to Attatch the Actor to the grid at the specified location, Assigns Grid Location and Parent Grid*/
	virtual ASquareGameGrid * AttachToGrid(FVector StartLocation, ASquareGameGrid * InGrid = nullptr);
	
	/*Moves the Actor and all primitive componenets to the specified tile if Valid*/
	virtual bool SetTileLocation(FGridTile Moveto);

	/*Returns Currently Connected Grid*/
	ASquareGameGrid * GetParentGrid() const;

	template < class T >
	T* GetParentGrid() const
	{
		return Cast<T>(GetParentGrid());
	}

	
	/*Gets the Tile the root component is currently occupying*/
	FGridTile GetRootGridTile() const;

	/*Tests whether the Actor and all registered primitives can be placed at the target location and remain on the Grid return false if any primitaves or root tile would land on invalid tiles*/
	virtual bool CanBePlacedAt(FGridTile TestTile) const;


	virtual ASquareGameGrid* FindGrid() const;

protected:
	virtual void SetParentGrid(ASquareGameGrid * InGrid);
	virtual void SavePrimitiveOffsets();
	virtual void SetRootGridTile(FGridTile InTile);


	/*Updates Registered Primative Components based on thier relative location to Grid Space Sets normal Relative Offset if tile is invalid*/
	virtual void UpdatePrimatives();

protected:
	virtual void PostInitializeComponents() override;
	virtual void OnConstruction(const FTransform & Transform) override;

protected:
	ASquareGameGrid * ParentGrid = nullptr;
	FGridTile RootGridLocation = FGridTile();
	
	bool bSavedOffsets = false;
	TMap<UPrimitiveComponent *, FVector> SavedLocalOffsets = TMap<UPrimitiveComponent *, FVector>();
	TMap<UPrimitiveComponent *, FGridTile> PrimativeTileLocations = TMap <UPrimitiveComponent *, FGridTile>();






};
