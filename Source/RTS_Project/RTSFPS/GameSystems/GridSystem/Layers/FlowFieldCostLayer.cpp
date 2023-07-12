// Fill out your copyright notice in the Description page of Project Settings.


#include "FlowFieldCostLayer.h"
#include "../GameGrid.h"


UFlowFieldCostLayer::UFlowFieldCostLayer()
{
	bIsLayerVisible = false;

	TraceQueryParams.TraceTag = "DebugCostTag";
	TraceQueryParams.bTraceComplex = false;
	TraceQueryParams.bReturnFaceIndex = true;
	TraceQueryParams.bReturnPhysicalMaterial = false;
	TraceQueryParams.bFindInitialOverlaps = false;
}

bool UFlowFieldCostLayer::GetTileCost(const UGridTile* InTile, uint8& OutCost) const
{
	bool retval = false;
	const uint8 * costptr = CostMap.Find(InTile);
	if (costptr != nullptr)
	{
		OutCost = *costptr;
		retval = true;
	}
	return retval;
}

void UFlowFieldCostLayer::PostActivateTile(UGridTile* InTile)
{
	const uint8 tilecost = CalculateTileCost(InTile);
	CostMap.Emplace(InTile, tilecost);
}

void UFlowFieldCostLayer::ShowTile(UGridTile* InTile)
{
	uint8 outcost;
	if (GetTileCost(InTile, outcost) && outcost > 0U)
	{
		FLinearColor fillcolor = GetCostFillColor(outcost);
		InTile->SetTileFillColor(fillcolor);
		InTile->SetTileVisible(true);
	}
}

void UFlowFieldCostLayer::HideTile(UGridTile* InTile)
{
	InTile->SetTileVisible(false);
}



void UFlowFieldCostLayer::CalculateTiles(const TArray<UGridTile*>& InTiles)
{
	for (int i = 0; i < InTiles.Num(); i++)
	{
		const uint8 tilecost = CalculateTileCost(InTiles[i]);
		CostMap.Emplace(InTiles[i], tilecost);
	}
}

uint8 UFlowFieldCostLayer::CalculateTileCost(const UGridTile* InTile)
{
	uint8 retval = FREE_TILE_COST;
	UWorld* world = GetWorld();

	if (world != nullptr)
	{
		static const FQuat tracerotation = FRotator(0.0f, 0.0f, 0.0f).Quaternion();
		static const FCollisionShape traceshape = FCollisionShape::MakeSphere(30.0f);

		const FVector tilenormal = InTile->GetTileNormal();
		const FVector tracelocation = InTile->GetTileCenter() + (tilenormal * BlockTraceHeightOffset);

		world->DebugDrawTraceTag = TraceQueryParams.TraceTag;

		FHitResult outhit = FHitResult();
		world->SweepSingleByChannel(outhit, tracelocation, tracelocation, tracerotation, BlockTraceChannel, traceshape, TraceQueryParams);

		if (outhit.bBlockingHit)
		{
			retval = BLOCKED_TILE_COST;
		}
	}

	return retval;
}

FLinearColor UFlowFieldCostLayer::GetCostFillColor(const uint8 InCost)
{
	static const float divisor = static_cast<float>(BLOCKED_TILE_COST);
	const float costfloat = static_cast<float>(InCost) - FREE_TILE_COST;
	const float intensity = costfloat / divisor;

	const FLinearColor retval = FLinearColor(intensity, 1.0f - intensity, 0.0f, 1.0f);

	return retval;
}
