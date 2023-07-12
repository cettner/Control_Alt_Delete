// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "../GridLayer.h"
#include "SuperTileManager.generated.h"


USTRUCT()
struct FSuperTile
{
	GENERATED_USTRUCT_BODY()

public:
	friend bool operator == (const FSuperTile& Myself, const FSuperTile& Other)
	{
		const bool retval = (Myself.AppliedLayer == Other.AppliedLayer);
		return retval;
	}

	friend bool operator == (const FSuperTile& Myself, const UGridLayer *& Other)
	{
		const bool retval = (Myself.AppliedLayer == Other);
		return retval;
	}

	UGridLayer * operator->() const {
		return AppliedLayer;
	}

public:
	UGridLayer* AppliedLayer = nullptr;

	TSet<UGridLayer*> TileNeighbors = TSet<UGridLayer*>();
};

UCLASS(BluePrintable)
class RTS_PROJECT_API USuperTileManager : public UGridLayer
{
	GENERATED_BODY()

	USuperTileManager();

public:
	FORCEINLINE TSubclassOf<UGridLayer> GetSuperTileClass() const { return LayerClass; }

protected:
	virtual void InitializeTileNeighbors(TArray<UGridLayer*>& InLayers);
	static 	FLinearColor GetTileHashColor(const int& InHashID);

protected:
	virtual void LayerInitialize(AGameGrid* InGrid, const TArray<UGridTile*>& InActiveTiles, AActor* InApplicator = nullptr) override;
	virtual void OnShowLayer() override;

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGridLayer> LayerClass = UGridLayer::StaticClass();

	UPROPERTY(EditDefaultsOnly)
	int32 RowSize = 2;

	UPROPERTY(EditDefaultsOnly)
	int32 ColomnSize = 2;

protected:
	TArray<FSuperTile> SuperTiles = TArray<FSuperTile>();
};
