// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "GridTile.h"

#include "GridLayer.generated.h"


constexpr int32 INVALID_LAYER_ID = 0xFFFFFFFF;

UCLASS()
class RTS_PROJECT_API UGridLayer : public UObject
{
	GENERATED_BODY()

	friend class AGameGrid;

public:
	int32 GetLayerID() const { return LayerID; }
	virtual void SetLayerVisibility(bool InbIsVisible);
	bool IsLayerVisible() const { return bIsLayerVisible; };
	bool IsLayerActivated() const { return bIsLayerActivated; }
	virtual void AddTile(UGridTile* InTile);
	virtual bool RemoveTile(UGridTile* InTile);
	FORCEINLINE bool ContainsTile(const UGridTile* InTile) const; 
	FORCEINLINE const TSet<UGridTile*>& GetTiles() const { return ActiveTileSet; }
	/*Swaps out the existing tileset for the provided one, if InbShouldActivateSharedTiles is set to true, all tiles 
	will be deativated and activated again even if they are contained in both sets*/
	virtual int ResetTiles(const TSet<UGridTile*>& InActiveTiles, const bool &InbShouldActivateSharedTiles = true);

	/*If the Grid is resp*/
	virtual TArray<UGridTile*> GetDefaultTileSet(const AGameGrid* InGrid) const;
	AGameGrid* GetGameGrid() const { return ParentGrid; }

	template<class T>
	T* GetGameGrid() const { Cast<T>(GetGameGrid()); }

protected:
	virtual void LayerInitialize(AGameGrid* InGrid, const TArray<UGridTile *>& InActiveTiles, AActor * InApplicator = nullptr);
	void SetLayerID(const int32 InID) { LayerID = InID;}
	UObject* GetApplicator() const { return Applicator;  }

	virtual void OnLayerActivate();
	virtual uint32 OnLayerDeactivate();
	
	virtual void OnShowLayer();
	virtual void OnHideLayer();

	virtual void PostActivateTile(UGridTile* InTile);
	virtual void PostDeactivateTile(UGridTile* InTile);
	virtual void ShowTile(UGridTile* InTile);
	virtual void HideTile(UGridTile* InTile);


protected:
	UPROPERTY(EditDefaultsOnly)
	bool bIsLayerVisible = false;

protected:
	/*Current TileSet, which is Duplicated in ActiveTileSet, as a result, always use AddTile and RemoveTile to change the layer*/
	TArray<UGridTile*> ActiveTiles = TArray<UGridTile*>();
	/*Sets cant be used for replication currently, but provide the fast access to the tileset, which should always be unique, this mirrors ActiveTiles*/
	TSet<UGridTile*> ActiveTileSet = TSet<UGridTile*>();

protected:
	AGameGrid* ParentGrid = nullptr;
	AActor * Applicator = nullptr;
	int32 LayerID = INVALID_LAYER_ID;
	bool bIsLayerActivated = false;

};
