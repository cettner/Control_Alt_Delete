// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "FlockAgentInterface.generated.h"


struct FObstacleTraceData
{
	//Length of the Trace to be performed
	float TraceLength = 0.0f;
	//Arc Size in Relative 
	float TraceArcSizeDegrees = 0.0f;
	// Number of traces performed in the Trace Arc, increasing this decreases the deviance in scanned regions
	int32 TraceCount = 0;
	//Collision Channel to Perform the Trace
	ECollisionChannel TraceChannel = ECollisionChannel::ECC_Pawn;

	static const FObstacleTraceData DefaultObstacleTraceData;
};

USTRUCT()
struct FObstacleScan
{
	GENERATED_USTRUCT_BODY()
public:
	float obstaclemagnitude = 0.0f;

	FVector collisionscore = FVector::ZeroVector;
	//if a dynamic obstacle is blocking the navigation path
	bool bisBlocked = false;
	//if true, the actor scanned is the goal or is on top of on the goal tile,
	bool bblockedgoal = false;

	void Reset()
	{
		bisBlocked = false;
		bblockedgoal = false;
		collisionscore = FVector::ZeroVector;
		obstaclemagnitude = 0.0;
	}

};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UFlockAgentInterface : public UInterface
{
	GENERATED_BODY()
};

class  IFlockAgentInterface
{
	GENERATED_BODY()

public:
	virtual const FObstacleTraceData& GetObstacleTraceData() const;

public:
	virtual const TSet<const UObject*> GetFlockAgents() const;
	virtual FVector GetFlockAgentLocation() const;
	virtual FVector GetFlockAgentDirection() const;
	virtual float GetFlockAgentRadius() const;
	virtual bool IsFlockMember(const AActor* InActor) const;

protected:
	virtual void UpdateFlockParameters();
	virtual FVector CalcSeparation();
	virtual FVector CalcAlignment();
	virtual FVector CalcCohesion();


/*Runtime*/
protected:
	FVector FlockSeparation = FVector::ZeroVector;
	FVector FlockAlignment = FVector::ZeroVector;
	FVector FlockCohesion = FVector::ZeroVector;
};
