// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidPartitionBounds.h"

// Sets default values
ABoidPartitionBounds::ABoidPartitionBounds()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	BoidBounds = CreateDefaultSubobject<UBoxComponent>(TEXT("RootBoundingArea"));
	RootComponent = BoidBounds;
}

void ABoidPartitionBounds::CalculateBoxCenters(const FVector& InBoxCenter, const FVector& InBoxExtent, const uint32 InNumLengthPartitions, const uint32 InNumWidthPartitions, const uint32 InNumHeightPartitions, TArray<FVector>& OutCenters, FVector& OutExtent)
{
    // Calculate the size of each partition along each dimension
    FVector PartitionSize(
     InBoxExtent.X * 2 / InNumLengthPartitions,
     InBoxExtent.Y * 2 / InNumWidthPartitions,
     InBoxExtent.Z * 2 / InNumHeightPartitions
      );

    // Calculate the starting corner of the partitioned space
    const FVector StartCorner = InBoxCenter - InBoxExtent;

    // Clear the output array
    OutCenters.Empty();

       // Loop through each partition along each dimension and calculate the center of each box
     for (uint32 Z = 0; Z < InNumHeightPartitions; ++Z)
     {
         for (uint32 Y = 0; Y < InNumWidthPartitions; ++Y)
         {
             for (uint32 X = 0; X < InNumLengthPartitions; ++X)
             {
                 // Calculate the center of the current box
                 FVector BoxCenter = StartCorner + FVector(
                     PartitionSize.X * (0.5f + X),
                     PartitionSize.Y * (0.5f + Y),
                     PartitionSize.Z * (0.5f + Z)
                 );

                 // Add the center to the output array
                 OutCenters.Add(BoxCenter);
             }
         }
     }

       // Calculate the extent of all the new boxes
       OutExtent = InBoxExtent / FVector(InNumLengthPartitions, InNumWidthPartitions, InNumHeightPartitions);
}

