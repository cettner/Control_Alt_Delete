// Fill out your copyright notice in the Description page of Project Settings.


#include "FlockPathFollowingComponent.h"
#include "RTS_Project/RTSFPS/RTS/RTSMinion.h"

FVector UFlockPathFollowingComponent::GetFlockAgentLocation() const
{
	FVector retval = FVector::ZeroVector;
	APawn * pawn = GetOwner<AAIController>()->GetPawn();

	if (pawn)
	{
		retval = pawn->GetActorLocation();
	}

	return retval;
}

FVector UFlockPathFollowingComponent::GetFlockAgentDirection() const
{
	FVector retval = FVector::ZeroVector;
	APawn* pawn = GetOwner<AAIController>()->GetPawn();

	if (pawn)
	{
		retval = pawn->GetActorForwardVector();
	}

	return retval;
}

void UFlockPathFollowingComponent::UpdatePathSegment()
{
	Super::UpdatePathSegment();

	const FVector cohesion = CalcCohesion();
	const FVector alignment = CalcAlignment();
	const FVector separation = CalcSeparation();

	FVector currentdirection = GetFlockAgentDirection();

	currentdirection += cohesion + alignment + separation;
	currentdirection.Normalize();

	FNavPathSharedPtr path = GetPath();
	FNavPathPoint& nextsegment = path->GetPathPoints()[MoveSegmentStartIndex + 1];
	nextsegment.Location += currentdirection;
}
