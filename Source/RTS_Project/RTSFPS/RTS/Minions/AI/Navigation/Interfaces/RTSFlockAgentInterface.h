// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "RTSFlockAgentInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class URTSFlockAgentInterface : public UInterface
{
	GENERATED_BODY()
};

/*Forward Declaration*/
class URTSFlock;

class RTS_PROJECT_API IRTSFlockAgentInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	FVector GetFlockAgentLocation() const;

	FVector GetFlockAgentVelocity() const;

	FVector GetFlockAgentDirection() const;

	inline URTSFlock* GetFlock() const { return CurrentFlock; }

public:
	virtual void ApplySeperation();
	virtual void ApplyAlignment();
	virtual void ApplyCohesion();

	virtual void ApplyObstacleAvoidance();

/*Runtime*/
protected:
	UPROPERTY()
	URTSFlock* CurrentFlock = nullptr;

	FVector SeperationForce = FVector::ZeroVector;
	FVector AlignmentForce = FVector::ZeroVector;
	FVector CohesionForce = FVector::ZeroVector;

};
