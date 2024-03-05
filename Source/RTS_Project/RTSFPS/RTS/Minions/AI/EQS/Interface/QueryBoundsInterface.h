// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EnvironmentQuery/EnvQueryContext.h"

#include "QueryBoundsInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UQueryBoundsInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RTS_PROJECT_API IQueryBoundsInterface
{
	GENERATED_BODY()

public:
	virtual bool IsPointWithinContextBounds(const FVector& InPoint, const TSubclassOf<UEnvQueryContext>& Context = nullptr) const;

};
