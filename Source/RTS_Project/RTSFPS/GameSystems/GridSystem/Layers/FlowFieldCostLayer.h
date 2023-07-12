// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../GridLayer.h"
#include "FlowFieldCostLayer.generated.h"

constexpr uint8 BLOCKED_TILE_COST = 0xFFU;
constexpr uint8 FREE_TILE_COST = 0x01U;

UCLASS()
class RTS_PROJECT_API UFlowFieldCostLayer : public UGridLayer
{
	GENERATED_BODY()

	friend class UFlowFieldSolutionLayer;

protected:
	UFlowFieldCostLayer();

public:
	virtual bool GetTileCost(const UGridTile* InTile, uint8& OutCost) const;
	FORCEINLINE const TMap<UGridTile*, uint8>& GetAllCosts() const { return CostMap; }

protected:
	virtual void PostActivateTile(UGridTile* InTile) override;
	virtual void ShowTile(UGridTile* InTile) override;
	virtual void HideTile(UGridTile* InTile) override;

protected:
	virtual void CalculateTiles(const TArray<UGridTile*>& InTiles);
	virtual uint8 CalculateTileCost(const UGridTile * InTile);
	virtual FLinearColor GetCostFillColor(const uint8 InCost);

protected:
	UPROPERTY(EditDefaultsOnly, Category = Setup)
	TEnumAsByte<ECollisionChannel> BlockTraceChannel = ECollisionChannel::ECC_Visibility;

	UPROPERTY(EditDefaultsOnly, Category = Setup)
	float BlockTraceHeightOffset = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category = Setup)
	float BlockTraceRadius = 30.0f;

	FCollisionQueryParams TraceQueryParams = FCollisionQueryParams::DefaultQueryParam;

protected:
	TMap<UGridTile *, uint8> CostMap = TMap<UGridTile *, uint8>();


};
