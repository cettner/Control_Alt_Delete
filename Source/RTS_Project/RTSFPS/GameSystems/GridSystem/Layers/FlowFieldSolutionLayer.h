// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NavigationSystemTypes.h"


#include "../GridLayer.h"
#include "FlowFieldCostLayer.h"
#include "FlowFieldIntegrationLayer.h"
#include "FlowFieldVectorLayer.h"
#include "FlowFieldSolutionLayer.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UFlowFieldSolutionLayer : public UGridLayer
{
	GENERATED_BODY()

	UFlowFieldSolutionLayer();

public:
	void SetGoalTile(const UGridTile* InTile, const bool bRebuildWeights = false);
	void SetGoalLocation(const FVector& InMoveLocation, const bool bRebuildWeights = false);
	void SetGoalActor(const AActor* InGoalActor, const bool IsDynamicGoal, const bool bRebuildWeights = false);
	virtual bool BuildSolution();
	virtual void ResetSolution();
	virtual void SubscribeAgent(const UObject * Subscriber);
	virtual void UnSubscribeAgent(const UObject* UnSubScriber);
	FORCEINLINE virtual bool IsAgentSubscribed(const UObject* UnSubScriber) const;

	bool HasGoal() const;
	FORCEINLINE bool IsGoalTile(const UGridTile * InGridTile) const;
	bool IsTileBlocked(const UGridTile* InGridTile) const;
	FORCEINLINE bool IsGoalActor(const AActor* InGoalActor) const { return GoalActor == InGoalActor; }
	FORCEINLINE bool GetFlowVectorForTile(const UGridTile* InTile, FVector& OutTile) const;
	FORCEINLINE bool GetWeightForTile(const UGridTile* InTile, float& Outweight) const;
	FORCEINLINE const UGridTile* GetGoalTile() const;
	FORCEINLINE bool IsGoalDynamic() const { return bIsGoalDynamic; };
	FORCEINLINE virtual bool NeedsRepath() const;
	FORCEINLINE const AActor* GetGoalActor() const { return GoalActor; }
	FORCEINLINE const FVector GetGoalLocation() const;
	virtual bool RequiresCostRebuild() const;
	virtual bool RequiresWeightRebuild() const;
	virtual bool IsSolutionReady() const;
	virtual bool CanUseSolutionforQuery(const struct FVectorFieldQuery& Query) const;


protected:
	virtual void LayerInitialize(AGameGrid* InGrid, const TArray<UGridTile*>& InActiveTiles, AActor* InApplicator) override;
	virtual void OnCostLayerRebuilt(UFlowFieldCostLayer * InRebuiltLayer);
	virtual bool InitializeCostData();
	virtual void OnLayerActivate() override;
	virtual uint32 OnLayerDeactivate() override;
	virtual void OnShowLayer() override;
	virtual void OnHideLayer() override;

	 
protected:
	/*Since Cost Data Exists in SuperTiles, we reassemble the cost data here from each of them*/
	TMap<UGridTile*, uint8> CostMap = TMap<UGridTile*, uint8>();

	UPROPERTY(transient)
	UFlowFieldIntegrationLayer* IntegrationLayer = nullptr;

	UPROPERTY(transient)
	UFlowFieldVectorLayer* VectorLayer = nullptr;

	TSet<const UObject*> Subscribers = TSet<const UObject*>();

	bool bIsGoalDynamic = false;

	const AActor* GoalActor = nullptr;

	FVector Goallocation = FVector();

	FVector LastUpdatedGoalPosition = FVector();

	/*Distance required from last position to initiate a Repath*/
	float RepathTetherDistance = 100.0f;

protected:
	bool bNeedsCostRebuild = true;
};