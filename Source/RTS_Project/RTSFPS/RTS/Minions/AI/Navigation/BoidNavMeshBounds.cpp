// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidNavMeshBounds.h"

void ABoidNavMeshBounds::BuildPartitions()
{
	//Create Partitions based on the set navigation bounds;
	// and add the TArray<UBoxComponent*> BoxPartitions = TArray<UBoxComponent*>()

	FBoxSphereBounds boundingarea =  GetBounds();
	FVector PartitionSize = boundingarea.BoxExtent / FVector(NumLengthPartitions, NumWidthPartitions, NumHeightPartitions);

	for (int32 i = 0; i < NumLengthPartitions; ++i)
	{
		for (int32 j = 0; j < NumWidthPartitions; ++j)
		{
			for (int32 k = 0; k < NumHeightPartitions; ++k)
			{
				// Calculate the position of the current partition
				const FVector PartitionLocation = boundingarea.Origin + FVector(i * PartitionSize.X, j * PartitionSize.Y, k * PartitionSize.Z);
				
				// Create a UBoxComponent for the current partition
				UBoxComponent* BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Bounds "));
				BoxComponent->SetWorldLocation(PartitionLocation);
				BoxComponent->InitBoxExtent(PartitionSize / 2); // Divide by 2 to get half-extent
				BoxComponent->RegisterComponent();

				// Add the BoxComponent to the array
				BoxPartitions.Add(BoxComponent);
			}
		}
	}

}
