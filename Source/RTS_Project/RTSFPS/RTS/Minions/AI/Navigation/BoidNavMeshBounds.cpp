// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidNavMeshBounds.h"

#include "Components/BrushComponent.h"

ABoidNavMeshBounds::ABoidNavMeshBounds() : Super()
{

}

void ABoidNavMeshBounds::BuildPartitions()
{
    // Get the vertices of the brush component
    TArray<FVector3f> points = GetBrushComponent()->Brush->Points;
    FBoxSphereBounds  bounds = GetBrushComponent()->Brush->Bounds;
    // Given the above is a set of 8 points for a box, subdivide the box into smaller boxes via the following, NumLengthPartitions, NumWidthPartitions, NumHeightPartitions Member variables
    
}

void ABoidNavMeshBounds::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	FlushPersistentDebugLines(GetWorld());
	BuildPartitions();

}
