// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSPathFollowingComponent.h"

#include "DrawDebugHelpers.h"
#include "NavigationSystem.h"
#include "AIController.h"

void URTSPathFollowingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (Status == EPathFollowingStatus::Moving)
	{
		FVector PathDirection = GetCurrentDirection();
		UpdateCollisions(PathDirection);

		if (!IsSteering())
		{
			// check finish conditions, update current segment if needed
			UpdatePathSegment();
			FollowPathSegment(DeltaTime);
		}
		else
		{
			UpdateSteeringPath();
			FollowPathSegment(DeltaTime);
		}

	}
}

void URTSPathFollowingComponent::UpdateCollisions(const FVector DesiredDirection)
{
	if (!Path.IsValid()) return;

	ScannedObstacles.Reset();
	static const FObstacleTraceData obstracedata = FObstacleTraceData::DefaultObstacleTraceData;

	const FVector agentlocation = GetFlockAgentLocation();
	const float agentradius = GetFlockAgentRadius();

	int32 halftracecount = obstracedata.TraceCount / 2;
	if (obstracedata.TraceCount % 2 == 0)
	{
		// Even number of traces, adjust half count
		halftracecount++;
	}
	const float MinOffset = -agentradius - (agentradius * .2);
	const float MaxOffset = agentradius + (agentradius * .2);

	for (int32 i = 0; i < obstracedata.TraceCount; i++)
	{
		const float Offset = FMath::Lerp(MinOffset, MaxOffset, static_cast<float>(i) / (obstracedata.TraceCount - 1));
		const FVector OffsetVector = FVector::CrossProduct(DesiredDirection, FVector::UpVector).GetSafeNormal() * Offset;
		const FVector TraceStart = agentlocation + OffsetVector;
		const FVector TraceEnd = TraceStart + DesiredDirection * obstracedata.TraceLength;

		// Perform the line trace and handle the results
		FHitResult HitResult;
		FCollisionQueryParams CollisionParams;
		CollisionParams.TraceTag = "DebugMovement";
		GetWorld()->DebugDrawTraceTag = CollisionParams.TraceTag;
		CollisionParams.AddIgnoredActor(GetOwner<AAIController>()->GetPawn()); // Ignore the owner of this component

		if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_GameTraceChannel5, CollisionParams))
		{
			if (HitResult.IsValidBlockingHit())
			{
				const AActor* hitactor = HitResult.GetActor();
				const float radius = hitactor->GetSimpleCollisionRadius();

				const FVector collisionvector = HitResult.Location - agentlocation;
				ScannedObstacles.bisBlocked = true;
				ScannedObstacles.bblockedgoal = Path->GetGoalActor() == HitResult.GetActor() && IsValid(HitResult.GetActor());
				ScannedObstacles.collisionscore += collisionvector;
				ScannedObstacles.obstaclemagnitude += radius;
			}
		}
	}
	ScannedObstacles.collisionscore.Normalize();
	const bool shouldusesteering = ShouldEnableSteering();
	SetIsSteering(shouldusesteering);
}

void URTSPathFollowingComponent::UpdatePathSegment()
{
#if !UE_BUILD_SHIPPING
	DEBUG_bMovingDirectlyToGoal = false;
#endif // !UE_BUILD_SHIPPING

	if ((Path.IsValid() == false) || (MovementComp == nullptr))
	{
		UE_CVLOG(Path.IsValid() == false, this, LogPathFollowing, Log, TEXT("Aborting move due to not having a valid path object"));
		OnPathFinished(EPathFollowingResult::Aborted, FPathFollowingResultFlags::InvalidPath);
		return;
	}

	const FVector CurrentLocation = MovementComp->GetActorFeetLocation();
	const bool bCanUpdateState = HasMovementAuthority();

	if (!Path->IsValid())
	{
		if (!Path->IsWaitingForRepath())
		{
			UE_VLOG(this, LogPathFollowing, Log, TEXT("Aborting move due to path being invalid and not waiting for repath"));
			OnPathFinished(EPathFollowingResult::Aborted, FPathFollowingResultFlags::InvalidPath);
			return;
		}
		else if (HasStartedNavLinkMove() && bCanUpdateState && Status == EPathFollowingStatus::Moving)
		{
			// pawn needs to get off navlink to unlock path updates (AAIController::ShouldPostponePathUpdates)
			if (HasReachedCurrentTarget(CurrentLocation))
			{
				OnSegmentFinished();
				SetNextMoveSegment();
			}
		}
		else
		{
			// continue with execution, if navigation is being rebuild constantly AI will get stuck with current waypoint
			// path updates should be still coming in, even though they get invalidated right away
			UE_VLOG(this, LogPathFollowing, Log, TEXT("Updating path points in invalid & pending path!"));
		}
	}

	/** it's possible that finishing this move request will result in another request
	 *	which won't be easily detectable from this function. This simple local
	 *	variable gives us this knowledge. */
	const FAIRequestID MoveRequestId = GetCurrentRequestId();

	// if agent has control over its movement, check finish conditions
	if (bCanUpdateState && Status == EPathFollowingStatus::Moving)
	{
		const int32 LastSegmentEndIndex = Path->GetPathPoints().Num() - 1;
		const bool bFollowingLastSegment = (MoveSegmentEndIndex >= LastSegmentEndIndex);

		if (bCollidedWithGoal)
		{
			// check if collided with goal actor
			OnSegmentFinished();
			OnPathFinished(EPathFollowingResult::Success, FPathFollowingResultFlags::None);
		}
		else if (MoveSegmentEndIndex > PreciseAcceptanceRadiusCheckStartNodeIndex && HasReachedDestination(CurrentLocation))
		{
			OnSegmentFinished();
			OnPathFinished(EPathFollowingResult::Success, FPathFollowingResultFlags::None);
		}
		else if (bFollowingLastSegment && bMoveToGoalOnLastSegment)
		{
			// use goal actor for end of last path segment
			// UNLESS it's partial path (can't reach goal)
			if (DestinationActor.IsValid() && Path->IsPartial() == false)
			{
				const FVector AgentLocation = DestinationAgent ? DestinationAgent->GetNavAgentLocation() : DestinationActor->GetActorLocation();
				// note that the condition below requires GoalLocation to be in world space.
				const FVector GoalLocation = FQuatRotationTranslationMatrix(DestinationActor->GetActorQuat(), AgentLocation).TransformPosition(MoveOffset);

				CurrentDestination.Set(NULL, GoalLocation);

				UE_VLOG(this, LogPathFollowing, Log, TEXT("Moving directly to move goal rather than following last path segment"));
				UE_VLOG_LOCATION(this, LogPathFollowing, VeryVerbose, GoalLocation, 30, FColor::Green, TEXT("Last-segment-to-actor"));
				UE_VLOG_SEGMENT(this, LogPathFollowing, VeryVerbose, CurrentLocation, GoalLocation, FColor::Green, TEXT_EMPTY);
			}

			UpdateMoveFocus();

#if !UE_BUILD_SHIPPING
			DEBUG_bMovingDirectlyToGoal = true;
#endif // !UE_BUILD_SHIPPING
		}
		// check if current move segment is finished
		else if (HasReachedCurrentTarget(CurrentLocation))
		{
			OnSegmentFinished();
			SetNextMoveSegment();
		}
	}

	if (bCanUpdateState
		&& Status == EPathFollowingStatus::Moving
		// still the same move request
		&& MoveRequestId == GetCurrentRequestId())
	{

		// gather location samples to detect if moving agent is blocked
		const bool bHasNewSample = UpdateBlockDetection();
		if (bHasNewSample && IsBlocked())
		{
			if (Path->GetPathPoints().IsValidIndex(MoveSegmentEndIndex) && Path->GetPathPoints().IsValidIndex(MoveSegmentStartIndex))
			{
			}
			else
			{
				if ((GetOwner() != NULL) && (MovementComp != NULL))
				{
					UE_VLOG(GetOwner(), LogPathFollowing, Verbose, TEXT("Path blocked, but move segment indices are not valid: start %d, end %d of %d"), MoveSegmentStartIndex, MoveSegmentEndIndex, Path->GetPathPoints().Num());
				}
			}
			OnPathFinished(EPathFollowingResult::Blocked, FPathFollowingResultFlags::None);
		}
	}
}

void URTSPathFollowingComponent::FollowPathSegment(float DeltaTime)
{
	if (!Path.IsValid() || MovementComp == nullptr)
	{
		return;
	}

	if (!IsSteering())
	{
		const FVector CurrentLocation = MovementComp->GetActorFeetLocation();
		const FVector CurrentTarget = GetCurrentTargetLocation();

		FVector MoveVelocity = (CurrentTarget - CurrentLocation) / DeltaTime;

		const int32 LastSegmentStartIndex = Path->GetPathPoints().Num() - 2;
		const bool bNotFollowingLastSegment = (MoveSegmentStartIndex < LastSegmentStartIndex);

		PostProcessMove.ExecuteIfBound(this, MoveVelocity);
		MovementComp->RequestDirectMove(MoveVelocity, bNotFollowingLastSegment);
	}
	else
	{
		const FVector CurrentLocation = MovementComp->GetActorFeetLocation();
		const FVector CurrentTarget = SteeringPath[SteeringIndex].Location;
		FVector MoveVelocity = (CurrentTarget - CurrentLocation) / DeltaTime;

		PostProcessMove.ExecuteIfBound(this, MoveVelocity);
		MovementComp->RequestDirectMove(MoveVelocity, false);
	}

}

bool URTSPathFollowingComponent::HasReachedCurrentTarget(const FVector& CurrentLocation) const
{
	if (MovementComp == NULL)
	{
		return false;
	}

	if (!IsSteering())
	{
		const FVector CurrentTarget = GetCurrentTargetLocation();
		const FVector CurrentDirection = GetCurrentDirection();

		// check if moved too far
		const FVector ToTarget = (CurrentTarget - MovementComp->GetActorFeetLocation());
		const FVector::FReal SegmentDot = FVector::DotProduct(ToTarget, CurrentDirection);
		if (SegmentDot < 0.)
		{
			return true;
		}

		// or standing at target position
		// don't use acceptance radius here, it has to be exact for moving near corners (2D test < 5% of agent radius)
		const float GoalRadius = 0.0f;
		const float GoalHalfHeight = 0.0f;

		return HasReachedInternal(CurrentTarget, GoalRadius, GoalHalfHeight, CurrentLocation, CurrentAcceptanceRadius, 0.05f);
	}
	else
	{

		const float GoalRadius = 0.0f;
		const float GoalHalfHeight = 0.0f;
		const FVector steeringtarget = SteeringPath[SteeringIndex].Location;
		return HasReachedInternal(steeringtarget, GoalRadius, GoalHalfHeight, CurrentLocation, CurrentAcceptanceRadius, 0.05f);
	}

}

void URTSPathFollowingComponent::Reset()
{
	Super::Reset();
	SetIsSteering(false);
}

float URTSPathFollowingComponent::GetFlockAgentRadius() const
{
	float retval = 0.0f;
	const APawn* pawn = GetOwner<AAIController>()->GetPawn();

	if (pawn)
	{
		retval = pawn->GetSimpleCollisionRadius();
	}

	return retval;
}

void URTSPathFollowingComponent::SetIsSteering(const bool InIsSteering)
{
	bIsSteering = InIsSteering;

	if (!bIsSteering)
	{
		SteeringPath.Reset();
		ScannedObstacles.Reset();
		SteeringIndex = INDEX_NONE;
	}
}

bool URTSPathFollowingComponent::ShouldEnableSteering() const
{
	bool retval = false;
	
	if (ScannedObstacles.bisBlocked)
	{
		if (ScannedObstacles.bblockedgoal)
		{
			retval = false;
		}
		else
		{
			retval = true;
		}
	}
	return retval;
}

void URTSPathFollowingComponent::UpdateSteeringPath()
{
	if (SteeringPath.Num() == 0)
	{
		UNavigationSystemV1* NavSys = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
		if (NavSys)
		{
			/*Get the Current Path Direction*/
			const FVector agentlocation = GetFlockAgentLocation();
			FVector goaldirection = GetCurrentDirection();
			FVector leftdirection = goaldirection.Cross(FVector::UpVector);
			FVector rightdirection = -leftdirection;
			FVector steersegment;

			const float leftscore = leftdirection.Dot(ScannedObstacles.collisionscore);
			const float rightscore = rightdirection.Dot(ScannedObstacles.collisionscore);

			if (leftscore > rightscore)
			{
				steersegment = agentlocation + (rightdirection * ScannedObstacles.obstaclemagnitude * 2.0f);
			}
			else
			{
				steersegment = agentlocation + (leftdirection * ScannedObstacles.obstaclemagnitude * 2.0f);
			}

			DrawDebugLine(GetWorld(), agentlocation, (steersegment), FColor::Red, true);
			DrawDebugLine(GetWorld(), steersegment, CurrentDestination.Position, FColor::Red, true);

			FNavPathPoint steerpoint = FNavPathPoint();
			if (NavSys->ProjectPointToNavigation(steersegment, steerpoint))
			{
				const FNavPathPoint startpoint = Path->GetPathPoints()[MoveSegmentStartIndex];
				SteeringPath.Emplace(startpoint);

				SteeringPath.Emplace(steerpoint);

				const FNavPathPoint endpoint = Path->GetPathPoints()[MoveSegmentEndIndex];
				SteeringPath.Emplace(endpoint);

				SteeringIndex = 1;
			}
		}
	}
	else
	{
		const FVector agentlocation = GetFlockAgentLocation();
		if (HasReachedCurrentTarget(agentlocation))
		{
			if (SteeringIndex == 2)
			{
				SetIsSteering(false);
			}
			else
			{
				SteeringIndex++;
			}
		}
	}
}

FVector URTSPathFollowingComponent::GetFlockAgentDirection() const
{
	FVector retval = FVector::ZeroVector;
	const APawn* pawn = GetOwner<AAIController>()->GetPawn();

	if (pawn)
	{
		retval = pawn->GetActorForwardVector();
	}

	return retval;
}

FVector URTSPathFollowingComponent::GetFlockAgentLocation() const
{
	FVector retval = FVector::ZeroVector;
	const APawn* pawn = GetOwner<AAIController>()->GetPawn();

	if (pawn)
	{
		retval = pawn->GetActorLocation();
	}

	return retval;
}