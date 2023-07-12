// Fill out your copyright notice in the Description page of Project Settings.


#include "FlowFieldIntegrationLayer.h"
#include "../GameGrid.h"



const UGridTile* UFlowFieldIntegrationLayer::GetEikonalPathTile(const UGridTile* InTile) const
{
	const UGridTile* retval = nullptr;
	if (const UGridTile* const* outtile = PathMap.Find(InTile))
	{
		retval = *outtile;
	}

	return retval;
}

bool UFlowFieldIntegrationLayer::GetTileWeight(const UGridTile* InTile, float& OutCost) const
{
	bool retval = false;
	const float* costptr = WeightMap.Find(InTile);
	if (costptr != nullptr)
	{
		OutCost = *costptr;
		retval = true;
	}
	return retval;
}

void UFlowFieldIntegrationLayer::SetGoalTile(const UGridTile* InTile)
{
	if (!IsGoalTile(InTile))
	{
		GoalTile = InTile;
		bRequiresRebuild = true;
	}
}

bool UFlowFieldIntegrationLayer::DoesGoalExist() const
{
	const bool retval = GoalTile != nullptr;
	return retval;
}

void UFlowFieldIntegrationLayer::OnLayerActivate()
{
	Super::OnLayerActivate();

	if (UseEikonalBuild())
	{
		BuildWeights_Eikonal();
	}
	else
	{
		BuildWeights();
	}


}

void UFlowFieldIntegrationLayer::ShowTile(UGridTile* InTile)
{
	float tileweight;
	const FVector tilecenter = InTile->GetTileCenter();
	const int32 tileid = InTile->GetTileID();
	if (GetTileWeight(InTile, tileweight))
	{
		const FString idstring = "ID : " + FString::FromInt(tileid) + "\n";
		FString weightstring;

		if (tileweight >= UNVISITED_TILE_WEIGHT)
		{
			weightstring = "Weight : MAX";
		}
		else
		{
			weightstring = "Weight : " + FString::SanitizeFloat(tileweight, 2);
		}

		const FString debugstring = idstring + weightstring;

		DrawDebugString(GetWorld(), tilecenter, debugstring, GetGameGrid(), FColor::Magenta);
	}
	else
	{
		DrawDebugString(GetWorld(), tilecenter, "ERROR", GetGameGrid(), FColor::Red);
	}

}

void UFlowFieldIntegrationLayer::HideTile(UGridTile* InTile)
{
	FlushDebugStrings(GetWorld());
}

void UFlowFieldIntegrationLayer::ResetWeights()
{
	WeightMap.Reset();
	GoalTile = nullptr;
	bRequiresRebuild = true;
}

void UFlowFieldIntegrationLayer::BuildWeights()
{
	/*set the value of all tiles except the goal(s) to max / unexplored value*/
	if (DoesGoalExist())
	{
		/*create a list of tiles that need to be explored, starting at the goals and branching out as the algorithm extends*/
		TQueue<const UGridTile*> openlist = TQueue<const UGridTile*>();
		/*The Queue class doesnt support internal retrival so this set lets us check whats in openlist*/
		TSet<const UGridTile*> visitedlist = TSet<const UGridTile*>();
		openlist.Enqueue(GoalTile);
		visitedlist.Emplace(GoalTile);

		for (int i = 0; i < ActiveTiles.Num(); i++)
		{
			WeightMap.Emplace(ActiveTiles[i], UNVISITED_TILE_WEIGHT);
		}

		WeightMap.Emplace(GoalTile, GOAL_TILE_WEIGHT);
		const UGridTile* roottile;
		/*While Tiles need to be explored, get the one at the front of the list*/
		while (openlist.Dequeue(roottile))
		{
			visitedlist.Remove(roottile);
			TArray<FGridTileNeighbor> neighbors = roottile->GetNeighbors();

			for (int i = 0; i < neighbors.Num(); i++)
			{
				UGridTile*& neighbor = neighbors[i].NeighborTile;
				float rootweight;
				float neighborweight; 
				uint8 neighborcost;
				float neighborcostfloat;

				/*Security Checking, make sure that the tiles being aquired are valid and costs have been computed  correctly*/
				const bool rootweightsuccess = GetTileWeight(roottile, rootweight);
				const bool neighborweightsuccess = GetTileWeight(neighbor, neighborweight);
				const bool neighborcostsuccess = GetTileCost(neighbor, neighborcost);
				const bool tilecheck = neighborweightsuccess && rootweightsuccess && neighborcostsuccess;

				neighborcostfloat = static_cast<float>(neighborcost);
				if (neighbors[i].bIsCornerNeighbor == true)
				{
					neighborweight += UE_SQRT_2;
					neighborcostfloat += UE_SQRT_2;
				}

				checkf(tilecheck, TEXT("UFlowFieldIntegrationLayer::BuildWeights : Invalid Tile retrieval for layer"))
				/*Skip any neighbor that is blocked*/
				if (neighborcost != BLOCKED_TILE_COST)
				{
					/*Compute the totalcost of the trip from the explored tile to the neighbor tile/*/
					const float totalcost = rootweight + neighborcostfloat;
					/*If the tile cost is lower than the current explored weight set the value*/
					if (totalcost < neighborweight)
					{
						/*if we're already going to explore it dont add it again*/
						if (!visitedlist.Contains(neighbor))
						{
							openlist.Enqueue(neighbor);
							visitedlist.Emplace(neighbor);
						}
						/*Assign the new Weight*/
						WeightMap.Emplace(neighbor, totalcost);
					}
				}
			}
		}
		bRequiresRebuild = false;
	}
}

void UFlowFieldIntegrationLayer::BuildWeights_Eikonal()
{
	TQueue<const UGridTile*> tileQueue = TQueue<const UGridTile*>();
	TSet<const UGridTile*> visitedlist = TSet<const UGridTile*>();

	PathMap.Reserve(ActiveTiles.Num());
	WeightMap.Reserve(ActiveTiles.Num());

	for (int i = 0; i < ActiveTiles.Num(); i++)
	{
		WeightMap.Emplace(ActiveTiles[i], UNVISITED_TILE_WEIGHT);
	}

	const UGridTile* goaltile = GetGoalTile();
	tileQueue.Enqueue(goaltile);
	WeightMap.Emplace(GoalTile, GOAL_TILE_WEIGHT);

	const UGridTile* CurrentTile = nullptr;
	while (tileQueue.Dequeue(CurrentTile))
	{
		const TArray<FGridTileNeighbor> neighbors = CurrentTile->GetNeighbors();

		for (int i = 0; i < neighbors.Num(); i++)
		{
			uint8 neighborcost = BLOCKED_TILE_COST;
			float currenttraveldistance = UNVISITED_TILE_WEIGHT;
			float neighbortraveldistance = UNVISITED_TILE_WEIGHT;

			UGridTile* const neighbor = neighbors[i].NeighborTile;
			const bool neighborcostsuccess = GetTileCost(neighbor, neighborcost);
			const bool currenttravelsuccess = GetTileWeight(CurrentTile, currenttraveldistance);
			const bool neighbortravelsuccess = GetTileWeight(neighbor, neighbortraveldistance);

			if (neighborcost != BLOCKED_TILE_COST)
			{
				const float tentativetime = currenttraveldistance + CalculateSpeed_Eikonal(CurrentTile, neighbors[i]);

				if (tentativetime < neighbortraveldistance)
				{
					WeightMap.Emplace(neighbor, tentativetime);
					PathMap.Emplace(neighbor, CurrentTile);

					if (!visitedlist.Contains(neighbor))
					{
						tileQueue.Enqueue(neighbor);
						visitedlist.Emplace(neighbor);
					}
				}
			}
		}
	}

	bRequiresRebuild = false;
}

float UFlowFieldIntegrationLayer::CalculateSpeed_Eikonal(const UGridTile * CurrentTile, const FGridTileNeighbor& InNeighbor) const
{
	float retval = 1.0;

	//FVector goaldirection = GetGoalTile()->GetTileCenter() - CurrentTile->GetTileCenter();
	//goaldirection.Normalize();
	//FVector tiledirection = InNeighbor.NeighborTile->GetTileCenter() - CurrentTile->GetTileCenter();
	//tiledirection.Normalize();

	//retval = 1 - FVector::DotProduct(goaldirection, tiledirection);

	return retval;
}

void UFlowFieldIntegrationLayer::SetCostData(const TMap<UGridTile*, uint8>& InCostMap)
{
	if (CostMapRef != &InCostMap)
	{
		CostMapRef = &InCostMap;
		bRequiresRebuild = true;
	}
}

bool UFlowFieldIntegrationLayer::GetTileCost(const UGridTile* InTile, uint8& OutCost) const
{
	checkf(CostMapRef, TEXT("UFlowFieldIntegrationLayer::GetTileCost CostMapRef was null"))
	bool retval = false;
	
	const uint8* costptr = CostMapRef->Find(InTile);
	if (costptr != nullptr)
	{
		OutCost = *costptr;
		retval = true;
	}

	return retval;
}
