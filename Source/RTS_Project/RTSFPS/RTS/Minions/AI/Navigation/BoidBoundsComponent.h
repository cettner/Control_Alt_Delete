// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"

#include "BoxPartitionComponent.h"
#include "BoidBoundsComponent.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UBoidBoundsComponent : public UBoxComponent
{
	GENERATED_BODY()

	UBoidBoundsComponent();

public:
	UBoxPartitionComponent* GetPartitionFromPosition(const FVector& InPosition) const;
	FORCEINLINE bool IsValidBoundsPoint(const FVector& InPosition) const;
	FORCEINLINE const FVector& GetMinBounds() const { return MinBounds; }
	FORCEINLINE const FVector& GetMaxBounds() const { return MaxBounds; }

	FORCEINLINE float GetPartitionLength() const { return (GetScaledBoxExtent().X / LengthPartitions * 2.0f); };
	FORCEINLINE float GetPartitionWidth() const { return (GetScaledBoxExtent().Y / LengthPartitions * 2.0f); }
	FORCEINLINE float GetPartitionHieght() const { return (GetScaledBoxExtent().Z / LengthPartitions * 2.0f); };
	FORCEINLINE FVector GetPartitionExtent() const { return GetScaledBoxExtent() / FVector(LengthPartitions, WidthPartitions, HeightPartitions); }

protected:
	virtual FPrimitiveSceneProxy* CreateSceneProxy() override;
	static void CalculateBoxCenters(const FVector& InBoxCenter, const FVector& InBoxExtent, const uint32 InNumLengthPartitions, const uint32 InNumWidthPartitions, const uint32 InNumHeightPartitons, TArray<FVector>& OutCenters, FVector& OutExtent);
	void InitializeBoundsNeighbors();
	
protected:
	virtual void BeginPlay() override;
	virtual void OnRegister() override;
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;

protected:
	UPROPERTY(EditAnyWhere, meta = (ClampMin = 1U))
	uint32 LengthPartitions = 2U;

	UPROPERTY(EditAnyWhere, meta = (ClampMin = 1U))
	uint32 HeightPartitions = 2U;

	UPROPERTY(EditAnyWhere, meta = (ClampMin = 1U))
	uint32 WidthPartitions = 2U;

	UPROPERTY(EditAnyWhere)
	bool bShowPartitions = true;

protected:
	float FractionalPartitionLength = 0.0f;
	float FractionalPartitionWidth = 0.0f;
	float FractionalPartitionHeight = 0.0f;
	FVector MinBounds = FVector();
	FVector MaxBounds = FVector();

protected:
	TArray<UBoxPartitionComponent*> BoundsPartitions = TArray<UBoxPartitionComponent*>();

	
};
