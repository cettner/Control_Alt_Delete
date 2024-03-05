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
			ARTSMinion * minion = CastChecked<ARTSMinion>(actors[i]);
			retval.Emplace(minion);
		}
	}

	return retval;
}

const TSet<ARTSMinion*> UBoidPathFollowingComponent::GetObstacleBoids() const
{
    TSet<ARTSMinion*> retval = TSet<ARTSMinion*>();
	const TArray<ARTSMinion*> neighbors = GetNeighboringBoids();
	const TSet<ARTSMinion*> flock = GetFlockingBoids();

	for (int i = 0; i < neighbors.Num(); i++)
	{
		if (!IsGoalActor(neighbors[i]))
		{
			if (!flock.Contains(neighbors[i]))
			{
				retval.Emplace(neighbors[i]);
			}
			else if (flock.Contains(neighbors[i]) && (neighbors[i]->GetVelocity() == FVector::ZeroVector))
			{
				retval.Emplace(neighbors[i]);
			}
		}
	}

	return retval;
}

const TSet<ARTSMinion*> UBoidPathFollowingComponent::GetFlockingBoids() const
{
	TSet<ARTSMinion*> retval = TSet<ARTSMinion*>();
	ARTSAIController * aic = GetOwner<ARTSAIController>();

	if (const URTSOrderGroup * ordergroup = aic->GetOrderGroup())
	{
		for (TScriptInterface<IRTSObjectInterface> object : ordergroup->GetAllActiveUnits())
		{
			if (ARTSMinion * minion = Cast<ARTSMinion>(object.GetObject()))
			{
				/*Skip ourselves*/
				if (minion != MovementComp->GetOwner<ARTSMinion>())
				{
					retval.Emplace(minion);
				}
			}
		}
	}

	return retval;
}

bool UBoidPathFollowingComponent::IsFlockMember(const ARTSMinion* InAgent) const
{
	return GetFlockingBoids().Contains(InAgent);
}

bool UBoidPathFollowingComponent::IsObstacleBoid(const ARTSMinion* InAgent) const
{
	return GetObstacleBoids().Contains(InAgent);
}

FVector UBoidPathFollowingComponent::CalculateSeperationForce(const TSet<ARTSMinion*>& InAvoidAgents) const
{
	FVector retval = FVector::ZeroVector;
	const FVector myagentlocation = MovementComp->GetOwner<INavAgentInterface>()->GetNavAgentLocation();

	for (const ARTSMinion* agent : InAvoidAgents)
	{
		const FVector otheragentlocation = agent->GetNavAgentLocation();
		const float distancescaling = FVector::DistSquared(myagentlocation, otheragentlocation);
		if (distancescaling > 0.0f)
		{
			retval += (MovementComp->GetActorFeetLocation() - agent->GetNavAgentLocation()) * (MaxSeperationForceDistSqrd / distancescaling);
		}
	}

	return retval;
}

FVector UBoidPathFollowingComponent::CalculateAlignmentForce(const TSet<ARTSMinion*>& InFlockAgents) const
{
	FVector retval = FVector::ZeroVector;
	int32 contributionnumber = 0;

	for (const ARTSMinion* minion : InFlockAgents)
	{
		const FVector velocity = minion->GetVelocity();
		if (velocity != FVector::ZeroVector)
		{
			retval += velocity;
			contributionnumber++;
		}
	}

	if (contributionnumber > 0)
	{
		retval /= contributionnumber;
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
	return SeperationForce + GoalForce + AlignmentForce;
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
		const TSet<ARTSMinion*> flock = GetFlockingBoids();
		const TSet<ARTSMinion*> obstacles = GetObstacleBoids();

		GoalForce = CalculateGoalForce() * GoalScaleFactor;
		SeperationForce = CalculateSeperationForce(obstacles) * SeperationFactor;
		AlignmentForce = CalculateAlignmentForce(flock) * AlignmentFactor;

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

	InDebug->ForceMap.Emplace("Seperation", GetSeperationForce());
	InDebug->ForceMap.Emplace("Goal", GetGoalForce());
	InDebug->ForceMap.Emplace("Boid", GetBoidForce());
	InDebug->ForceMap.Emplace("Alignment", GetAlignmentForce());

	for (int i = 0; i < neighbors.Num(); i++)
	{
		FColor boidlinecolor = FColor::Blue;
		if (IsObstacleBoid(neighbors[i]))
		{
			boidlinecolor = FColor::Red;
		}
		else if (IsFlockMember(neighbors[i]))
		{
			boidlinecolor = FColor::Green;
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

		if (AlignmentForce != FVector::ZeroVector)
		{
			InDebug->AddShape(FGameplayDebuggerShape::MakeArrow(agentlocation, agentlocation + AlignmentForce, 6.0f, 6.0f, FColor::Magenta));
		}

		if (BoidForce != FVector::ZeroVector)
		{
			InDebug->AddShape(FGameplayDebuggerShape::MakeArrow(agentlocation, agentlocation + BoidForce, 6.0f, 6.0f, FColor::Yellow));
		}


	}

}