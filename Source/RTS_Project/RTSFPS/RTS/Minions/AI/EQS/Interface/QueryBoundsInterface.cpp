// Fill out your copyright notice in the Description page of Project Settings.


#include "QueryBoundsInterface.h"

// Add default functionality here for any IQueryBoundsInterface functions that are not pure virtual.

bool IQueryBoundsInterface::IsPointWithinContextBounds(const FVector& InPoint, const TSubclassOf<UEnvQueryContext>& Context) const
{
    return false;
}
