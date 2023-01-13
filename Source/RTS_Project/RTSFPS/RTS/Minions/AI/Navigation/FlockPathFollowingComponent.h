// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Navigation/PathFollowingComponent.h"

#include "Interfaces/RTSFlockAgentInterface.h"
#include "FlockPathFollowingComponent.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UFlockPathFollowingComponent : public UPathFollowingComponent, public IRTSFlockAgentInterface
{
	GENERATED_BODY()

	
};
