// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Navigation/PathFollowingComponent.h"
#include "RTS_Project/RTSFPS/GameSystems/GridSystem/Navigation/Interfaces/FlockAgentInterface.h"


#include "RTSPathFollowingComponent.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API URTSPathFollowingComponent : public UPathFollowingComponent, public IFlockAgentInterface
{
	GENERATED_BODY()
	

protected:
	virtual void UpdateCollisions(const FVector DesiredDirection);

protected:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void UpdatePathSegment() override;
	virtual void FollowPathSegment(float DeltaTime) override;
	virtual bool HasReachedCurrentTarget(const FVector& CurrentLocation) const override;
	virtual void Reset() override;

protected:
	FVector GetFlockAgentLocation() const override;
	FVector GetFlockAgentDirection() const override;
	virtual float GetFlockAgentRadius() const override;

protected:
	void SetIsSteering(const bool InIsSteering);
	FVector GetSteeringDirection() const;
	virtual bool ShouldEnableSteering() const;
	virtual void UpdateSteeringPath();
	FORCEINLINE bool IsSteering() const { return bIsSteering; }

protected:
	FObstacleScan ScannedObstacles = FObstacleScan();

	bool bIsSteering = false;

	TArray<FNavPathPoint> SteeringPath = TArray<FNavPathPoint>();

	FVector SteeringDirection = FVector();

	int SteeringIndex = INDEX_NONE;
};
