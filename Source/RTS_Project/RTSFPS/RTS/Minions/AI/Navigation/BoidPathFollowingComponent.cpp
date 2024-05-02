// Fill out your copyright notice in the Description page of Project Settings.


#include "BoidPathFollowingComponent.h"

#include "Kismet/GameplayStatics.h"

UBoidPathFollowingComponent::UBoidPathFollowingComponent()
{
	BoidForce = FVector::ZeroVector;
	MaxSeperationForceDistSqrd = FMath::Square(MaxSeperationForceDistance);
}

UBoxPartitionComponent* UBoidPathFollowingComponent::GetCurrentNavPartition() const
{
	UBoxPartitionComponent* retval = nullptr;
	if (ABoidPartitionBounds* partitionbounds = GetPartitionBounds())
	{
		retval = partitionbounds->GetPartitionFromPosition(MovementComp->GetActorFeetLocation());
	}

	return retval;
}

TSet<UBoxPartitionComponent*> UBoidPathFollowingComponent::GetRelevantPartitions(float InFilterAngle) const
{
	TSet<UBoxPartitionComponent*> retval = TSet<UBoxPartitionComponent*>();
	if (UBoxPartitionComponent* currentbounds = GetCurrentNavPartition())
	{
		retval.Emplace(currentbounds);
		const FVector& currentdirection = GetOwner<AAIController>()->GetControlRotation().Vector();
		const FVector& currentlocation = MovementComp->GetActorFeetLocation();

		for (UBoxPartitionComponent* neighborbounds : currentbounds->GetNeighbors())
		{
			const FVector& positiondiff = neighborbounds->GetComponentLocation() - currentlocation;
			// Calculate the angle between the direction vector and the position difference vector
			const float angle = FMath::RadiansToDegrees(FMath::Acos(currentdirection.GetSafeNormal().Dot(positiondiff.GetSafeNormal())));

			// Check if the angle is within the filter angle
			if (angle <= InFilterAngle)
			{
				retval.Emplace(neighborbounds);
			}
		}

	}
	return retval;
}

ABoidPartitionBounds* UBoidPathFollowingComponent::GetPartitionBounds() const
{
	ABoidPartitionBounds* retval = Cast<ABoidPartitionBounds>(UGameplayStatics::GetActorOfClass(GetWorld(), ABoidPartitionBounds::StaticClass()));
	return retval;
}

const TSet<ARTSMinion*> UBoidPathFollowingComponent::GetObstacleBoids() const
{
    TSet<ARTSMinion*> retval = TSet<ARTSMinion*>();
	const TSet<ARTSMinion*> neighbors = GetNeighboringBoids();
	const TSet<ARTSMinion*> flock = GetFlockingBoids();
	const FVector& mydirection = GetCurrentDirection();
	const FVector& myposition = MovementComp->GetActorFeetLocation();

	for (ARTSMinion* neighbor : neighbors)
	{
		if (!IsGoalActor(neighbor))
		{
			const FVector& neighbordirection = neighbor->GetNavAgentLocation() - myposition;

			if (!flock.Contains(neighbor) && neighbordirection.Dot(mydirection) >= 0.0f)
			{
				retval.Emplace(neighbor);
			}
			/*If A Flockmate is stationary or moving the opposite direction for some reason, they're an obstacle*/
			else if (flock.Contains(neighbor) && (neighbor->GetVelocity().Dot(mydirection) <= 0.0f))
			{
				retval.Emplace(neighbor);
			}
		}
	}

	return retval;
}

const TSet<ARTSMinion*> UBoidPathFollowingComponent::GetFlockingBoids(const bool InOnlyLocalNeighbors) const
{
	TSet<ARTSMinion*> retval = TSet<ARTSMinion*>();
	ARTSAIController * aic = GetOwner<ARTSAIController>();

	if (const URTSOrderGroup * ordergroup = aic->GetOrderGroup())
	{
		const TSet<TScriptInterface<IRTSObjectInterface>, FInterfaceObjectHash> orderunits = ordergroup->GetAllActiveUnits();

		if (InOnlyLocalNeighbors)
		{
			const TSet<ARTSMinion*>& neighbors = GetNeighboringBoids();
			 
			if (neighbors.Num() == 0 || orderunits.Num() == 1) return retval;

			if (neighbors.Num() <= orderunits.Num())
			{
				for (ARTSMinion* object : neighbors)
				{
					if (orderunits.Contains(TScriptInterface<IRTSObjectInterface>(object)) && (object != MovementComp->GetOwner()))
					{
						retval.Emplace(CastChecked<ARTSMinion>(object));
					}
				}
			}
			else
			{
				for (const TScriptInterface<IRTSObjectInterface>& object : orderunits)
				{
					if (neighbors.Contains(Cast<ARTSMinion>(object.GetObject())) && (object.GetObject() != MovementComp->GetOwner()))
					{
						retval.Emplace(CastChecked<ARTSMinion>(object.GetObject()));
					}
				}
			}

		}
		else
		{
			if (orderunits.Num() == 1) return retval;

			for (TScriptInterface<IRTSObjectInterface> object : ordergroup->GetAllActiveUnits())
			{
				if (ARTSMinion* minion = Cast<ARTSMinion>(object.GetObject()))
				{
					/*Skip ourselves*/
					if (minion != MovementComp->GetOwner<ARTSMinion>())
					{
						retval.Emplace(minion);
					}
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

bool UBoidPathFollowingComponent::ShouldUseBoidSteering() const
{
	const TSet<ARTSMinion*>& surroundingagents = GetNeighboringBoids();
	bool retval = surroundingagents.Num() > 0;
	return retval;
}

void UBoidPathFollowingComponent::UpdateBoidNeighbors()
{
	NeighboringBoids.Reset();
	const TSet<UBoxPartitionComponent* >& navpartitions = GetRelevantPartitions(180.0f);

	for (UBoxPartitionComponent* mypartition : navpartitions)
	{
		for (AActor* partitionactor : mypartition->GetActors())
		{
			if (IsValid(partitionactor) && partitionactor != MovementComp->GetOwner())
			{
				NeighboringBoids.Emplace(CastChecked<ARTSMinion>(partitionactor));
			}
		}
	}
}

FVector UBoidPathFollowingComponent::CalculateSeperationForce(const TSet<ARTSMinion*>& InAvoidAgents) const
{
	FVector retval = FVector::ZeroVector;
	const FVector myagentlocation = MovementComp->GetOwner<INavAgentInterface>()->GetNavAgentLocation();

	for (const ARTSMinion* agent : InAvoidAgents)
	{
		const FVector& otheragentlocation = agent->GetNavAgentLocation();
		const float& distancescaling = FVector::DistSquared(myagentlocation, otheragentlocation);
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
		const FVector& velocity = minion->GetVelocity();
		const FVector& mydirection = GetCurrentDirection();
		const float dotproduct = velocity.Dot(mydirection);
		if (dotproduct > 0.0f)
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

	const FVector& currentLocation = MovementComp->GetActorFeetLocation();
	const FVector& currentTarget = GetCurrentTargetLocation();

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
		UpdateBoidNeighbors();
		if (ShouldUseBoidSteering())
		{
			const TSet<ARTSMinion*>& flock = GetFlockingBoids();
			const TSet<ARTSMinion*>& obstacles = GetObstacleBoids();

			GoalForce = CalculateGoalForce() * GoalScaleFactor;
			SeperationForce = CalculateSeperationForce(obstacles) * SeperationFactor;
			AlignmentForce = CalculateAlignmentForce(flock) * AlignmentFactor;

			if (SeperationForce.SquaredLength() > GoalForce.SquaredLength())
			{
				SeperationForce = SeperationForce.GetSafeNormal() * GoalForce.Length();
			}

			BoidForce = CalculateBoidForce();
			UpdateMoveFocus();
		}
		else
		{
			GoalForce = FVector::ZeroVector;
			BoidForce = FVector::ZeroVector;
		}
	}


}

void UBoidPathFollowingComponent::FollowPathSegment(float DeltaTime)
{
	if (!Path.IsValid() || MovementComp == nullptr)
	{
		return;
	}

	const FVector& CurrentLocation = MovementComp->GetActorFeetLocation();
	const FVector& CurrentTarget = GetCurrentTargetLocation();

	const FVector& boidsteering = GetBoidForce();
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

FVector UBoidPathFollowingComponent::GetMoveFocus(bool bAllowStrafe) const
{
	FVector retval = FVector();
	const FVector& boidsteering = GetBoidForce();
	const FVector& CurrentLocation = MovementComp->GetActorFeetLocation();

	if (boidsteering != FVector::ZeroVector)
	{
		retval = CurrentLocation + boidsteering * 10.0f;
	}
	else
	{
		retval = Super::GetMoveFocus(bAllowStrafe);
	}


	return retval;
}

void UBoidPathFollowingComponent::DescribeSelfToGameplayDebugger(FGameplayDebuggerCategory_Boid* InDebug)
{
	const TSet<ARTSMinion*> neighbors = GetNeighboringBoids();
	const FVector agentlocation = MovementComp->GetActorFeetLocation();


	InDebug->ForceMap.Emplace("Seperation", GetSeperationForce());
	InDebug->ForceMap.Emplace("Goal", GetGoalForce());
	InDebug->ForceMap.Emplace("Boid", GetBoidForce());
	InDebug->ForceMap.Emplace("Alignment", GetAlignmentForce());

	if (UBoxPartitionComponent* mypartition = GetCurrentNavPartition())
	{
		const FVector& boxcenter = mypartition->GetComponentLocation();
		const FVector& extent = mypartition->GetUnscaledBoxExtent();
		InDebug->AddShape(FGameplayDebuggerShape::MakeBox(boxcenter, extent, FColor::Green));


		TSet<UBoxPartitionComponent*> partitionset = GetRelevantPartitions();
		for (UBoxPartitionComponent* neighborpartition : partitionset)
		{
			if (neighborpartition != mypartition)
			{
				const FVector& nieghborcenter = neighborpartition->GetComponentLocation();
				const FVector& nieghborextent = neighborpartition->GetUnscaledBoxExtent();
				InDebug->AddShape(FGameplayDebuggerShape::MakeBox(nieghborcenter, nieghborextent, FColor::Blue));
			}
		}
	}



	for (ARTSMinion* neighbor : neighbors)
	{
		FColor boidlinecolor = FColor::Blue;
		if (IsObstacleBoid(neighbor))
		{
			boidlinecolor = FColor::Red;
		}
		else if (IsFlockMember(neighbor))
		{
			boidlinecolor = FColor::Green;
		}

		InDebug->AddShape(FGameplayDebuggerShape::MakeSegment(agentlocation, neighbor->GetNavAgentLocation(), boidlinecolor));

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