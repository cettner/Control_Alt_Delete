// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "../GridLayer.h"
#include "FlowFieldIntegrationLayer.h"
#include "FlowFieldVectorLayer.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UFlowFieldVectorLayer : public UGridLayer
{
	GENERATED_BODY()

	friend class UFlowFieldSolutionLayer;

protected:
	virtual void OnLayerActivate() override;
	virtual void ShowTile(UGridTile* InTile) override;
	virtual void HideTile(UGridTile* InTile) override;

public:
	virtual bool GetTileVector(const UGridTile* InTile, FVector& OutVector) const;


protected:
	UFlowFieldIntegrationLayer* GetIntegrationLayer() const;
	void SetIntegrationLayer(UFlowFieldIntegrationLayer* InIntegrationLayer);
	virtual void BuildFlowField();
	virtual void BuildFlowField_Eikonal();
	virtual void ResetFlowField();

protected:
	TMap<UGridTile*, FVector> FlowMap = TMap<UGridTile*, FVector>();

	UFlowFieldIntegrationLayer* IntegrationLayer = nullptr;
};
