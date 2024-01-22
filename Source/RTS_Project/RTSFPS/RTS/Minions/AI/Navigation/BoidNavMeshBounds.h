// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "Components/BoxComponent.h"

#include "BoidNavMeshBounds.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API ABoidNavMeshBounds : public ANavMeshBoundsVolume
{
	GENERATED_BODY()

	virtual void BuildPartitions();

protected:
	UPROPERTY(EditAnyWhere, Category = "Navigation")
	int32 NumLengthPartitions = 2;

	UPROPERTY(EditAnyWhere, Category = "Navigation")
	int32 NumWidthPartitions = 2;

	UPROPERTY(EditAnyWhere, Category = "Navigation")
	int32 NumHeightPartitions = 2;

	TArray<UBoxComponent*> BoxPartitions = TArray<UBoxComponent*>();
};
