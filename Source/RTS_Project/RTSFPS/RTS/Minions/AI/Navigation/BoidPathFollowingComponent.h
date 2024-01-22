// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Navigation/PathFollowingComponent.h"


#include "../../../RTSMinion.h"
#include "Debug/GameplayDebuggerCategory_Boid.h"

#include "BoidPathFollowingComponent.generated.h"





UCLASS()
class RTS_PROJECT_API UBoidPathFollowingComponent : public UPathFollowingComponent
{
	GENERATED_BODY()

public:
	UBoidPathFollowingComponent();

protected:
	const TArray<ARTSMinion*> GetNeighboringBoids() const;
	bool ShouldSeperateFrom(ARTSMinion* InAgent, float& outdistsquared) const;
	bool ShouldSeperateFrom(ARTSMinion* InAgent) const;
	FORCEINLINE FVector GetSeperationForce() const { return SeperationForce; }
	FORCEINLINE FVector GetGoalForce() const { return GoalForce; }
	FORCEINLINE FVector GetBoidForce() const { return BoidForce; }
	FVector CalculateSeperationForce(const TArray<ARTSMinion*>& InAvoidAgents) const;
	FVector CalculateGoalForce() const;
	FVector CalculateBoidForce() const;

protected:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void Reset() override;
	virtual void UpdatePathSegment() override;
	virtual void FollowPathSegment(float DeltaTime) override;
	virtual void OnPathFinished(const FPathFollowingResult& Result) override;


public:
	virtual void DescribeSelfToGameplayDebugger(FGameplayDebuggerCategory_Boid* InDebug);



public:
	FVector BoidForce = FVector::ZeroVector;
	FVector SeperationForce = FVector::ZeroVector;
	FVector GoalForce = FVector::ZeroVector;
	float MaxSeperationForceDistance = 200.0f;

	float GoalScaleFactor = 250.0f;
	float SeperationFactor = .33f;

protected:
	float MaxSeperationForceDistSqrd;
};
