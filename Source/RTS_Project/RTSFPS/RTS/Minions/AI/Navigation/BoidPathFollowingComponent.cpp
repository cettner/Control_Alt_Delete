// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidPathFollowingComponent.h"

#include "Kismet/GameplayStatics.h"

UBoidPathFollowingComponent::UBoidPathFollowingComponent()
{
	BoidForce = FVector::ZeroVector;
	MaxSeperationForceDistSqrd = FMath::Square(MaxSeperationForceDistance);
}


const TArray<ARTSMinion*> UBoidPathFollowingComponent::GetNeighboringBoids() const
{
	TArray<ARTSMinion*> retval = TArray<ARTSMinion*>();
	TArray<AActor*> actors = TArray<AActor*>();
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARTSMinion::StaticClass(), actors);

	for (int i = 0; i < actors.Num(); i++)
	{
		/*Make sure we dont include ourselves*/
		if (actors[i] != MovementComp->GetOwner())
		{
			retval.Emplace(CastChecked<ARTSMinion>(actors[i]));
		}
	}

	return retval;
}

bool UBoidPathFollowingComponent::ShouldSeperateFrom(ARTSMinion* InAgent, float& outdistsquared) const
{
	bool retval = false;
	const FVector myagentlocation = MovementComp->GetOwner<INavAgentInterface>()->GetNavAgentLocation();
	const FVector otheragentlocation = InAgent->GetNavAgentLocation();

	outdistsquared = FVector::DistSquared(myagentlocation, otheragentlocation);
	if(outdistsquared <= MaxSeperationForceDistSqrd)
	{
		retval = true;
	}

	return true;
}

bool UBoidPathFollowingComponent::ShouldSeperateFrom(ARTSMinion* InAgent) const
{
	float throwaway = 0.0f;
	return ShouldSeperateFrom(InAgent,throwaway);
}

FVector UBoidPathFollowingComponent::CalculateSeperationForce(const TArray<ARTSMinion*>& InAvoidAgents) const
{
	FVector retval = FVector::ZeroVector;

	for (int i = 0; i < InAvoidAgents.Num(); i++)
	{
		float distancescaling = 0.0f;
		if (ShouldSeperateFrom(InAvoidAgents[i], distancescaling))
		{
			retval += (MovementComp->GetActorFeetLocation() - InAvoidAgents[i]->GetNavAgentLocation()) * (MaxSeperationForceDistSqrd / distancescaling);
		}
	}

	return retval;
}

FVector UBoidPathFollowingComponent::CalculateGoalForce() const
{
	FVector retval = FVector::ZeroVector;

	const FVector currentLocation = MovementComp->GetActorFeetLocation();
	const FVector currentTarget = GetCurrentTargetLocation();

	/*
	const float distSquared = FVector::DistSquared(currentLocation, currentTarget);
	float squaredgoaldistance = FMath::Square(15000.0f);

	if (Path.IsValid())
	{
		const FNavigationPath* PathInstance = Path.Get();
		const FNavPathPoint& PathPt0 = PathInstance->GetPathPoints()[MoveSegmentStartIndex];
		const FNavPathPoint& PathPt1 = PathInstance->GetPathPoints()[MoveSegmentEndIndex];

		squaredgoaldistance = FVector::DistSquared(PathPt0.Location, PathPt1.Location);
	}


	if (distSquared > 0.0f)
	{
		retval = ((currentTarget - currentLocation).GetSafeNormal() * (squaredgoaldistance / distSquared));
	}
	*/

	retval = (currentTarget - currentLocation).GetSafeNormal();
	return retval;
}

FVector UBoidPathFollowingComponent::CalculateBoidForce() const
{
	return SeperationForce + GoalForce;
}

void UBoidPathFollowingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UBoidPathFollowingComponent::Reset()
{
	Super::Reset();
	BoidForce = FVector::ZeroVector;
	GoalForce = FVector::ZeroVector;
	SeperationForce = FVector::ZeroVector;
}

void UBoidPathFollowingComponent::UpdatePathSegment()
{
	Super::UpdatePathSegment();

	if (Status == EPathFollowingStatus::Moving)
	{
		const TArray<ARTSMinion*> boidneighbors = GetNeighboringBoids();
		GoalForce = CalculateGoalForce() * GoalScaleFactor;
		SeperationForce = CalculateSeperationForce(boidneighbors) * SeperationFactor;

		if (SeperationForce.SquaredLength() > GoalForce.SquaredLength())
		{
			SeperationForce = SeperationForce.GetSafeNormal() * GoalForce.Length();
		}

		BoidForce = CalculateBoidForce();
	}

}

void UBoidPathFollowingComponent::FollowPathSegment(float DeltaTime)
{
	if (!Path.IsValid() || MovementComp == nullptr)
	{
		return;
	}

	const FVector CurrentLocation = MovementComp->GetActorFeetLocation();
	const FVector CurrentTarget = GetCurrentTargetLocation();

	FVector boidsteering = GetBoidForce();
	FVector MoveVelocity;
	if (boidsteering != FVector::ZeroVector)
	{
		MoveVelocity = boidsteering / DeltaTime;
	}
	else
	{
		MoveVelocity = (CurrentTarget - CurrentLocation) / DeltaTime;
	}

	const int32 LastSegmentStartIndex = Path->GetPathPoints().Num() - 2;
	const bool bNotFollowingLastSegment = (MoveSegmentStartIndex < LastSegmentStartIndex);

	PostProcessMove.ExecuteIfBound(this, MoveVelocity);
	MovementComp->RequestDirectMove(MoveVelocity, bNotFollowingLastSegment);
}

void UBoidPathFollowingComponent::OnPathFinished(const FPathFollowingResult& Result)
{
	Super::OnPathFinished(Result);
}

void UBoidPathFollowingComponent::DescribeSelfToGameplayDebugger(FGameplayDebuggerCategory_Boid* InDebug)
{
	const TArray<ARTSMinion*> neighbors = GetNeighboringBoids();
	const FVector agentlocation = MovementComp->GetActorFeetLocation();
	const FVector serperationforce = GetSeperationForce();
	const FVector goalforce = GetGoalForce();

	InDebug->ForceMap.Emplace("Seperation", serperationforce);
	InDebug->ForceMap.Emplace("Goal", goalforce);
	InDebug->ForceMap.Emplace("Boid", GetBoidForce());


	for (int i = 0; i < neighbors.Num(); i++)
	{
		FColor boidlinecolor = FColor::Blue;
		if (ShouldSeperateFrom(neighbors[i]))
		{
			boidlinecolor = FColor::Red;
		}

		InDebug->AddShape(FGameplayDebuggerShape::MakeSegment(agentlocation, neighbors[i]->GetNavAgentLocation(), boidlinecolor));

		if (GoalForce != FVector::ZeroVector)
		{
			InDebug->AddShape(FGameplayDebuggerShape::MakeArrow(agentlocation, agentlocation + GoalForce, 6.0f, 6.0f, FColor::Green));
		}

		if (SeperationForce != FVector::ZeroVector)
		{
			InDebug->AddShape(FGameplayDebuggerShape::MakeArrow(agentlocation, agentlocation + SeperationForce, 6.0f, 6.0f, FColor::Red));
		}

		if (BoidForce != FVector::ZeroVector)
		{
			InDebug->AddShape(FGameplayDebuggerShape::MakeArrow(agentlocation, agentlocation + BoidForce, 6.0f, 6.0f, FColor::Yellow));
		}
	}

}