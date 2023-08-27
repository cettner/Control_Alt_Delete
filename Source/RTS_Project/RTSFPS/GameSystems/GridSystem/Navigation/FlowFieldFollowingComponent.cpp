// Fill out your copyright notice in the Description page of Project Settings.


#include "FlowFieldFollowingComponent.h"
#include "../GameGrid.h"

#include "BrainComponent.h"
#include "AIController.h"

constexpr int32 AT_GOAL = 0;
constexpr int32 NOT_AT_GOAL = -1;

void UFlowFieldFollowingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (Status == EPathFollowingStatus::Moving)
	{
		FVector desiredflowfieldirection;
		if (GetCurrentFlowFieldVector(desiredflowfieldirection))
		{
			UpdateObstacles(desiredflowfieldirection);
			UpdateFlock(desiredflowfieldirection);
		}

		if (IsUsingSteering())
		{
			UpdateMovementTiles();
		}
	}

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

UFlowFieldSolutionLayer* UFlowFieldFollowingComponent::GetFlowFieldSolution() const
{
	UFlowFieldSolutionLayer* retval = nullptr;
	if (Path.IsValid())
	{
		const FVectorFieldPath* fieldpath = Path->CastPath<FVectorFieldPath>();
		const AGameGrid* grid = CastChecked<AGameGrid>(fieldpath->GetNavigationDataUsed());
		retval = grid->GetLayerByIndex<UFlowFieldSolutionLayer>(fieldpath->GetSolutionID());
	}

	return retval;
}

bool UFlowFieldFollowingComponent::GetCurrentFlowFieldVector(FVector& OutVector)
{
	bool success = false;
	const UFlowFieldSolutionLayer* solution = GetFlowFieldSolution();
	if (IsValid(solution))
	{
		const AGameGrid* grid = solution->GetGameGrid();
		const FVector currentlocation = MovementComp->GetActorFeetLocation();
		const UGridTile* currenttile = grid->GetTileFromLocation(currentlocation);

		if (currenttile != nullptr)
		{
			success = solution->GetFlowVectorForTile(currenttile, OutVector);
		}
	}

	return success;
}

void UFlowFieldFollowingComponent::SetSteeringTile(const UGridTile* InTile)
{
	SteeringTile = InTile;
	SteeringTileAttemptMoves = 0;
	if (InTile != nullptr)
	{
		AGameGrid* grid = InTile->GetGameGrid();
		grid->SetTileColor(InTile->GetTileID(), FLinearColor::Blue);
		grid->SetTileVisible(InTile->GetTileID(), true);
	}

}

bool UFlowFieldFollowingComponent::ShouldEnableSteering() const
{
	bool retval = false;
	if (SteeringTile != nullptr  && !ScannedObstacles.bblockedgoal)
	{
		retval = true;
	}
	else if (ScannedObstacles.bisBlocked)
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

bool UFlowFieldFollowingComponent::IsActorAtGoal(const AActor* InActor) const
{
	bool retval = false;
	const UFlowFieldSolutionLayer* solution = GetFlowFieldSolution();

	if (IsValid(InActor) && IsValid(solution))
	{
		const AActor * goalactor = Path->GetGoalActor();
		
		/*Dynamic goal*/
		if (IsValid(goalactor))
		{
			/*TODO: add crowdtouching implementation which would be an else if off the solution subscriber list*/
			retval = (InActor == goalactor);
		}
		else
		{
			const AGameGrid* grid = solution->GetGameGrid();
			const FVector actorlocation = InActor->GetActorLocation();
			const UGridTile* actortile = grid->GetTileFromLocation(actorlocation);
			if (solution->IsGoalTile(actortile))
			{
				retval = true;
			}
		}
	}

	return retval;
}

void UFlowFieldFollowingComponent::UpdateMovementTiles()
{
	if (Path.IsValid())
	{
		const FVectorFieldPath* fieldpath = Path->CastPath<FVectorFieldPath>();
		const AGameGrid* grid = CastChecked<AGameGrid>(fieldpath->GetNavigationDataUsed());

		if (CurrentTile == nullptr)
		{
			CurrentTile = grid->GetTileFromLocation(GetFlockAgentLocation());
		}
		else
		{
			const UGridTile* currenttile = grid->GetTileFromLocation(GetFlockAgentLocation());
			const bool issteeringtileblocked = SteeringTileAttemptMoves >= MaxSteeringAttempts;
			if (CurrentTile == SteeringTile)
			{
				SetSteeringTile(nullptr);
			}
			else if (issteeringtileblocked)
			{
				RejectedSteeringTiles.Emplace(SteeringTile);
				SetSteeringTile(nullptr);
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Reseting Steering Tile")));
			}

			if (CurrentTile != currenttile)
			{
				PreviousTile = CurrentTile;
				CurrentTile = currenttile;
			}
		}
	}

}

void UFlowFieldFollowingComponent::UpdateObstacles(const FVector DesiredDirection)
{
	ScannedObstacles.Reset();
	const FObstacleTraceData obstracedata = GetObstacleTraceData();
	const FVector agentlocation = GetFlockAgentLocation();
	const float agentradius = GetFlockAgentRadius();


	int32 halftracecount = obstracedata.TraceCount / 2;
	if (obstracedata.TraceCount % 2 == 0)
	{
		// Even number of traces, adjust half count
		halftracecount++;
	}
	const float MinOffset = -agentradius - (agentradius *.2);
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

		if (GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, COLLISION_DYNAMIC_OBSTACLE, CollisionParams))
		{
			if (HitResult.IsValidBlockingHit())
			{
				const FVector collisionvector = HitResult.Location - agentlocation;
				ScannedObstacles.bisBlocked = true;
				ScannedObstacles.bblockedgoal |= IsActorAtGoal(HitResult.GetActor());
				ScannedObstacles.collisionscore += collisionvector;
				ScannedObstacles.collisionscore.Normalize();
			}
		}
	}

	const bool shouldusesteering = ShouldEnableSteering();
	SetUseSteering(shouldusesteering);
}

void UFlowFieldFollowingComponent::UpdateFlock(const FVector DesiredDirection)
{

}

void UFlowFieldFollowingComponent::SetUseSteering(const bool InUseSteering)
{
	if (!InUseSteering && bIsUsingSteering)
	{
		CurrentTile = nullptr;
		PreviousTile = nullptr;
		SetSteeringTile(nullptr);
		RejectedSteeringTiles.Reset();
	}
	bIsUsingSteering = InUseSteering;
}

void UFlowFieldFollowingComponent::ApplyObstacleSteering(FVector& OutBaseVector, const UFlowFieldSolutionLayer* InSolution)
{

	if (SteeringTile != nullptr)
	{
		FVector steerdirection = (SteeringTile->GetTileCenter() - CurrentTile->GetTileCenter()).GetSafeNormal();
		OutBaseVector = steerdirection;
		SteeringTileAttemptMoves++;
	}
	// Check if there are any scanned obstacles that aren't standing on the goal
	else if (ScannedObstacles.bisBlocked  && !ScannedObstacles.bblockedgoal)
	{

		TArray<const UGridTile*> alternatetiles = GetAlternateTileMoves(OutBaseVector, InSolution);
		if (alternatetiles.Num())
		{
			const UGridTile * chosentile = ChooseBestSteeringTile(alternatetiles, OutBaseVector);
			SetSteeringTile(chosentile);
			if (IsValid(SteeringTile))
			{
				const FVector steerdirection = (SteeringTile->GetTileCenter() - CurrentTile->GetTileCenter()).GetSafeNormal();
				OutBaseVector = steerdirection;
			}

		}
	}
}

TArray<const UGridTile*> UFlowFieldFollowingComponent::GetAlternateTileMoves(const FVector& IntendedDirection, const UFlowFieldSolutionLayer* InSolution) const
{
	TArray<const UGridTile*> retval = TArray<const UGridTile*>();
	const TArray<FGridTileNeighbor> neighboringtiles = CurrentTile->GetNeighbors();

	for (int i = 0; i < neighboringtiles.Num(); i++)
	{
		const UGridTile* neighbortile = neighboringtiles[i].NeighborTile;
		retval.Add(neighbortile);
	}

	return retval;
}

const UGridTile* UFlowFieldFollowingComponent::ChooseBestSteeringTile(TArray<const UGridTile*>& InTiles,const FVector& InDesiredDirection) const
{
	const UGridTile* BestTile = nullptr;
	double BestScore = -FLT_MAX;

	for (const UGridTile* Tile : InTiles)
	{
		if (!RejectedSteeringTiles.Contains(Tile))
		{
			// Calculate a score for the tile based on your desired criteria, including the DesiredDirection
			const double Score = CalculateSteeringTileScore(Tile, InDesiredDirection);

			// Update the best tile if the current tile has a higher score
			if (Score > BestScore)
			{
				// Perform the line trace and handle the results
				const FObstacleTraceData obstracedata = GetObstacleTraceData();
				FHitResult HitResult;
				FCollisionQueryParams CollisionParams;
				CollisionParams.TraceTag = "DebugMovement";
				GetWorld()->DebugDrawTraceTag = CollisionParams.TraceTag;
				CollisionParams.AddIgnoredActor(GetOwner<AAIController>()->GetPawn()); // Ignore the owner of this component

				const FVector TraceStart = GetFlockAgentLocation();
				FVector tilecenter = Tile->GetTileCenter();
				tilecenter.Z = TraceStart.Z;
				const FVector TraceDirection = (tilecenter - TraceStart).GetSafeNormal();
				const FVector TraceEnd = TraceStart + (TraceDirection * obstracedata.TraceLength);
				GetWorld()->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, obstracedata.TraceChannel, CollisionParams);

				if (!HitResult.bBlockingHit)
				{
					BestTile = Tile;
					BestScore = Score;
				}
			}
		}
	}

	return BestTile;
}

const float UFlowFieldFollowingComponent::CalculateSteeringTileScore(const UGridTile* Tile, const FVector& DesiredDirection) const
{
	const UFlowFieldSolutionLayer* solution = GetFlowFieldSolution();
	const UGridTile* goaltile = solution->GetGoalTile();
	const FVector agentlocation = GetFlockAgentLocation();
	const FVector goaldirection = goaltile->GetTileCenter() - agentlocation;
	const FVector TileDirection = (Tile->GetTileCenter() - CurrentTile->GetTileCenter()).GetSafeNormal();


	// Calculate the dot product of the tile flow field vector and desired direction
	
	float tileweightscore = 0.0f;
	float currentweight = 0.0f;
	float otherweight = 0.0f;



	if (solution->GetWeightForTile(Tile, otherweight)  && solution->GetWeightForTile(Tile, currentweight))
	{
		if (otherweight <= currentweight)
		{
			tileweightscore = 1.0f;
		}
		else
		{
			tileweightscore = currentweight / otherweight;
		}
	} 

	// Calculate the dot product of the tile direction and desired direction
	const float DirectionDotProduct = FVector::DotProduct(TileDirection, DesiredDirection);

	const float GoalDirectionDotProduct = FVector::DotProduct(TileDirection, goaldirection);

	const float CollisionScoreDotProduct = FVector::DotProduct(TileDirection, ScannedObstacles.collisionscore);

	const float PreviousMotionDotProduct = FVector::DotProduct(TileDirection, PreviousMovement);


	// Return the sum of the dot products as the tile score with weighted biasing
	const float retval = (DirectionDotProduct *  .3) + (tileweightscore *.9) + PreviousMotionDotProduct + (GoalDirectionDotProduct *.7) - CollisionScoreDotProduct;
	return retval;
}

void UFlowFieldFollowingComponent::ApplyFlockSteering(FVector& InBaseVector)
{
}

FVector UFlowFieldFollowingComponent::GetFlockAgentLocation() const
{
	FVector retval = FVector::ZeroVector;
	const APawn * pawn = GetOwner<AAIController>()->GetPawn();

	if (pawn)
	{
		retval = pawn->GetActorLocation();
	}

	return retval;
}

FVector UFlowFieldFollowingComponent::GetFlockAgentDirection() const
{
	FVector retval = FVector::ZeroVector;
	const APawn* pawn = GetOwner<AAIController>()->GetPawn();

	if (pawn)
	{
		retval = pawn->GetActorForwardVector();
	}

	return retval;
}

bool UFlowFieldFollowingComponent::IsFlockMember(const AActor* InActor) const
{
	bool retval = false;
	if (Path.IsValid())
	{
		if(const APawn* pawn = Cast<APawn>(InActor))
		{
			const AAIController* aic = pawn->GetController<AAIController>();
			if (IsValid(aic))
			{
				const FVectorFieldPath* fieldpath = Path->CastPath<FVectorFieldPath>();
				const AGameGrid* grid = Cast<AGameGrid>(fieldpath->GetNavigationDataUsed());
				const UFlowFieldSolutionLayer* solution = grid->GetLayerByIndex<UFlowFieldSolutionLayer>(fieldpath->GetSolutionID());
				if (IsValid(solution))
				{
					retval = solution->IsAgentSubscribed(aic->GetPathFollowingComponent());
				}
			}
		}
	}
	return retval;
}

const TSet<const UObject*> UFlowFieldFollowingComponent::GetFlockAgents() const
{
	return TSet<const UObject*>();
}

float UFlowFieldFollowingComponent::GetFlockAgentRadius() const
{
	float retval = 0.0f;
	const APawn* pawn = GetOwner<AAIController>()->GetPawn();

	if (pawn)
	{
		retval = pawn->GetSimpleCollisionRadius();
	}

	return retval;
}

void UFlowFieldFollowingComponent::OnPathFinished(const FPathFollowingResult& Result)
{
	if (Path.IsValid())
	{
		FVectorFieldPath* fieldpath = Path->CastPath<FVectorFieldPath>();
		AGameGrid* grid = Cast<AGameGrid>(fieldpath->GetNavigationDataUsed());
		UFlowFieldSolutionLayer* solution = grid->GetLayerByIndex<UFlowFieldSolutionLayer>(fieldpath->GetSolutionID());
		solution->UnSubscribeAgent(this);
	}

	const FAIRequestID FinishedMoveId = GetCurrentRequestId();

	Reset();

	OnRequestFinished.Broadcast(FinishedMoveId, Result);
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Ending Task %d"), FinishedMoveId));
	FAIMessage Msg(UBrainComponent::AIMessage_MoveFinished, this, FinishedMoveId, Result.IsSuccess());
	Msg.SetFlag(Result.Flags & 0xff);
	FAIMessage::Send(Cast<AController>(GetOwner()), Msg);

	/*
	FString stringreason = "NAN";

	switch (Result.Code)
	{
	case EPathFollowingResult::Aborted :
		stringreason = "Aborted";
		break;
	case EPathFollowingResult::Blocked :
		stringreason = "Blocked";
		//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("PathComplete Reason: %s"), *stringreason));
		break;
	case EPathFollowingResult::Success :
		stringreason = "Success";
		break;

	default:
		break;
	}
	*/

}

void UFlowFieldFollowingComponent::UpdatePathSegment()
{
	if ((Path.IsValid() == false) || (MovementComp == nullptr))
	{
		//UE_CVLOG(Path.IsValid() == false, this, LogPathFollowing, Log, TEXT("Aborting move due to not having a valid path object"));
		UPathFollowingComponent::OnPathFinished(EPathFollowingResult::Aborted, FPathFollowingResultFlags::InvalidPath);
		return;
	}

	const FVector CurrentLocation = MovementComp->GetActorFeetLocation();
	const bool bCanUpdateState = HasMovementAuthority();

	if (!Path->IsValid())
	{
		if (!Path->IsWaitingForRepath())
		{
			//UE_VLOG(this, LogPathFollowing, Log, TEXT("Aborting move due to path being invalid and not waiting for repath"));
			//UPathFollowingComponent::OnPathFinished(EPathFollowingResult::Aborted, FPathFollowingResultFlags::InvalidPath);
			return;
		}
		else
		{
			// continue with execution, if navigation is being rebuild constantly AI will get stuck with current waypoint
			// path updates should be still coming in, even though they get invalidated right away
			//UE_VLOG(this, LogPathFollowing, Log, TEXT("Updating path points in invalid & pending path!"));
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
		//const bool bLastPathChunk = (MetaNavPath == nullptr || MetaNavPath->IsLastSection());

		if (bCollidedWithGoal)
		{
			// check if collided with goal actor
			OnSegmentFinished();
			UPathFollowingComponent::OnPathFinished(EPathFollowingResult::Success, FPathFollowingResultFlags::None);
		}
	}
		// check if current move segment is finished
	else if (HasReachedCurrentTarget(CurrentLocation))
	{
		OnSegmentFinished();
		SetNextMoveSegment();
	}

	if (bCanUpdateState && Status == EPathFollowingStatus::Moving && MoveRequestId == GetCurrentRequestId())
	{
	// gather location samples to detect if moving agent is blocked
		const bool bHasNewSample = UpdateBlockDetection();
		if (bHasNewSample && IsBlocked())
		{
			UPathFollowingComponent::OnPathFinished(EPathFollowingResult::Blocked, FPathFollowingResultFlags::None);
		}
	}
}

void UFlowFieldFollowingComponent::OnActorBump(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if (IsActorAtGoal(OtherActor))
	{
		bCollidedWithGoal = true;
	}
}

FVector UFlowFieldFollowingComponent::GetMoveFocus(bool bAllowStrafe) const
{
	if (PreviousMovement != FVector::ZeroVector)
	{
		return PreviousMovement;
	}
	else 
	{
		return FAISystem::InvalidLocation;
	}

}

void UFlowFieldFollowingComponent::UpdateMoveFocus()
{
	AAIController* AIOwner = Cast<AAIController>(GetOwner());
	if (AIOwner != NULL)
	{
		if (Status == EPathFollowingStatus::Moving)
		{
			const FVector MoveFocus = GetMoveFocus(AIOwner->bAllowStrafe);
			AIOwner->SetFocalPoint(MoveFocus, EAIFocusPriority::Move);
		}
		else if (Status == EPathFollowingStatus::Idle)
		{
			AIOwner->ClearFocus(EAIFocusPriority::Move);
		}
	}
}

void UFlowFieldFollowingComponent::FollowPathSegment(float DeltaTime)
{
	if (!Path.IsValid() || MovementComp == nullptr)
	{
		return;
	}

	const FVector CurrentLocation = MovementComp->GetActorFeetLocation();
	const FVector CurrentTarget = GetCurrentTargetLocation();

	const UFlowFieldSolutionLayer* solution = GetFlowFieldSolution();

	if (IsValid(solution))
	{
		const FVectorFieldPath* fieldpath = Path->CastPath<FVectorFieldPath>();
		const AGameGrid* grid = CastChecked<AGameGrid>(fieldpath->GetNavigationDataUsed());
		const UGridTile* currenttile = grid->GetTileFromLocation(CurrentLocation);

		FVector outvelocityvector;

		if (solution->GetFlowVectorForTile(currenttile, outvelocityvector))
		{
			ApplyObstacleSteering(outvelocityvector, solution);
			PostProcessMove.ExecuteIfBound(this, outvelocityvector);
			MovementComp->RequestDirectMove(outvelocityvector, true);
			PreviousMovement = outvelocityvector;
		}
		else if (!solution->GetGoalActor() && HasReachedDestination(solution->GetGoalLocation()))
		{
			bCollidedWithGoal = true;
		}
		else if(solution->IsGoalTile(currenttile))
		{
			FVector goaldirection = solution->GetGoalLocation() - CurrentLocation;
			goaldirection.GetSafeNormal();
			MovementComp->RequestDirectMove(goaldirection, true);
			PreviousMovement = goaldirection;
		}
		else
		{
			Path.Reset();
		}
	}
	else
	{
		Path.Reset();
	}
}

FAIRequestID UFlowFieldFollowingComponent::RequestMove(const FAIMoveRequest& RequestData, FNavPathSharedPtr InPath)
{
	FAIRequestID retval = FAIRequestID();

	FVectorFieldPath* fieldpath = InPath->CastPath<FVectorFieldPath>();
	AGameGrid* grid = Cast<AGameGrid>(fieldpath->GetNavigationDataUsed());
	const int32 layerid = fieldpath->GetSolutionID();
	UFlowFieldSolutionLayer* solution = grid->GetLayerByIndex<UFlowFieldSolutionLayer>(layerid);

	if (IsValid(solution))
	{
		solution->SubscribeAgent(this);
		retval = Super::RequestMove(RequestData, InPath);

	}
	else
	{
		retval = FAIRequestID::InvalidRequest;
	}

	return retval;
}

void UFlowFieldFollowingComponent::Reset()
{
	Super::Reset();
	CurrentTile = nullptr;
	PreviousTile = nullptr;
	SteeringTile = nullptr;

	PreviousMovement = FVector::ZeroVector;
	ScannedObstacles.Reset();
	bIsUsingSteering = false;
	
}
