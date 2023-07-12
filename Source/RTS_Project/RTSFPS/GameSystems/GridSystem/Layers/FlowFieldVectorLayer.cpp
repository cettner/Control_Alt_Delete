// Fill out your copyright notice in the Description page of Project Settings.


#include "FlowFieldVectorLayer.h"
#include "../GameGrid.h"

#include "DrawDebugHelpers.h"

void UFlowFieldVectorLayer::OnLayerActivate()
{
	Super::OnLayerActivate();
	if (IntegrationLayer->UseEikonalBuild())
	{
		BuildFlowField_Eikonal();
	}
	else
	{
		BuildFlowField();
	}

}

void UFlowFieldVectorLayer::ShowTile(UGridTile* InTile)
{
	FVector flowvector;

	if (GetTileVector(InTile, flowvector))
	{
		const FVector elevationoffset = FVector(0, 0, 10.0f);
		const float arrowsize = 500.0f;
		const FVector startpoint = InTile->GetTileCenter() + elevationoffset;
		const FVector endpoint = (startpoint + (flowvector * 70.0f));

		DrawDebugDirectionalArrow(GetWorld(), startpoint, endpoint, arrowsize, FColor::Black, true, -1.0f, 0U, 10.0f);
	}


}

void UFlowFieldVectorLayer::HideTile(UGridTile* InTile)
{

}

bool UFlowFieldVectorLayer::GetTileVector(const UGridTile* InTile, FVector& OutVector) const
{
	bool retval = false;
	const FVector* costptr = FlowMap.Find(InTile);
	if (costptr != nullptr)
	{
		OutVector = *costptr;
		retval = true;
	}
	return retval;
}

UFlowFieldIntegrationLayer* UFlowFieldVectorLayer::GetIntegrationLayer() const
{
	return IntegrationLayer;
}

void UFlowFieldVectorLayer::SetIntegrationLayer(UFlowFieldIntegrationLayer* InIntegrationLayer)
{
	IntegrationLayer = InIntegrationLayer;
}

void UFlowFieldVectorLayer::BuildFlowField()
{
	UFlowFieldIntegrationLayer* integrationlayer = GetIntegrationLayer();

	for (const auto tile : integrationlayer->GetTiles())
	{
		if (!integrationlayer->IsGoalTile(tile))
		{
			const TArray<FGridTileNeighbor> neighbors = tile->GetNeighbors();

			float runningmin = UNVISITED_TILE_WEIGHT;
			float currentweight;
			UGridTile* runningtile = nullptr;
			for (int i = 0; i < neighbors.Num(); i++)
			{
				UGridTile* currentneighbor = neighbors[i].NeighborTile;
				integrationlayer->GetTileWeight(currentneighbor, currentweight);
				if (currentweight < runningmin)
				{
					runningmin = currentweight;
					runningtile = currentneighbor;
				}
			}

			/*Can Occur if surounding tiles are blocked / unvisited*/
			if (runningtile != nullptr)
			{
				const FVector intilevector = tile->GetTileCenter();
				const FVector neighborvector = runningtile->GetTileCenter();
				FVector returnvector = neighborvector - intilevector;
				returnvector.Normalize();

				FlowMap.Emplace(tile, returnvector);
			}
		}
	}
}

void UFlowFieldVectorLayer::BuildFlowField_Eikonal()
{
	UFlowFieldIntegrationLayer* integrationlayer = GetIntegrationLayer();

	for (const auto tile : integrationlayer->GetTiles())
	{
		if (!integrationlayer->IsGoalTile(tile))
		{
			const UGridTile* pointertile = integrationlayer->GetEikonalPathTile(tile);
			if (pointertile != nullptr)
			{
				const FVector intilevector = tile->GetTileCenter();
				const FVector neighborvector = pointertile->GetTileCenter();
				FVector returnvector = neighborvector - intilevector;
				returnvector.Normalize();
				FlowMap.Emplace(tile, returnvector);
			}
		}
	}

}

void UFlowFieldVectorLayer::ResetFlowField()
{
	FlowMap.Reset();
}
