// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidPartitionBounds.h"

// Sets default values
ABoidPartitionBounds::ABoidPartitionBounds()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	BoidBounds = CreateDefaultSubobject<UBoidBoundsComponent>(TEXT("RootBoundingArea"));
	RootComponent = BoidBounds;
}

UBoxPartitionComponent* ABoidPartitionBounds::GetPartitionFromPosition(const FVector& InPosition) const
{
    return BoidBounds->GetPartitionFromPosition(InPosition);
}