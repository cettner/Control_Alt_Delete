// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Navigation/PathFollowingComponent.h"

#include "../Layers/FlowFieldSolutionLayer.h"
#include "Interfaces/FlockAgentInterface.h"
#include "FlowFieldFollowingComponent.generated.h"


#define COLLISION_DYNAMIC_OBSTACLE ECC_GameTraceChannel5

USTRUCT()
struct FObstacleScan
{
	GENERATED_USTRUCT_BODY()
public:
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
	}

};


UCLASS()
class UFlowFieldFollowingComponent : public UPathFollowingComponent, public IFlockAgentInterface
{
	GENERATED_BODY()
		
	protected:
		FORCEINLINE UFlowFieldSolutionLayer* GetFlowFieldSolution() const;
		FORCEINLINE bool GetCurrentFlowFieldVector(FVector& OutVector);
		FORCEINLINE bool IsUsingSteering() const { return bIsUsingSteering; }
		FORCEINLINE void SetSteeringTile(const UGridTile* InTile);
		virtual bool ShouldEnableSteering() const;
		virtual void UpdateMovementTiles();
		virtual void UpdateObstacles(const FVector DesiredDirection);
		virtual void UpdateFlock(const FVector DesiredDirection);

		virtual void SetUseSteering(const bool InUseSteering);
		virtual void ApplyObstacleSteering(FVector& InBaseVector, const UFlowFieldSolutionLayer* InSolution);
		virtual TArray<const UGridTile*> GetAlternateTileMoves(const FVector& IntendedDirection, const UFlowFieldSolutionLayer* InSolution) const;
		virtual const UGridTile* ChooseBestSteeringTile(TArray<const UGridTile*>& InTiles,const FVector& InDesiredDirection) const;
		virtual const float CalculateSteeringTileScore(const UGridTile* Tile, const FVector& DesiredDirection) const;
		virtual void ApplyFlockSteering(FVector& InBaseVector);
		//Returns the number of adjectent actors the provided actor is touching in distance to the goal, 0 is at the goal and NOT_AT_GOAL for the actor has no relation to the goal.
		virtual bool IsActorAtGoal(const AActor* InActor) const;

	protected:
		virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	protected:
		FVector GetFlockAgentLocation() const override;
		FVector GetFlockAgentDirection() const override;
		bool IsFlockMember(const AActor* InActor) const override;
		const TSet<const UObject*> GetFlockAgents() const override;
		virtual float GetFlockAgentRadius() const;

	protected:
		/** notify about finishing move along current path segment */
		void OnPathFinished(const FPathFollowingResult& Result) override;
		virtual void UpdatePathSegment() override;
		/** follow current path segment */
		virtual void FollowPathSegment(float DeltaTime) override;
		virtual FAIRequestID RequestMove(const FAIMoveRequest& RequestData, FNavPathSharedPtr InPath) override;
		virtual void Reset() override;
		virtual void OnActorBump(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit) override;
		virtual FVector GetMoveFocus(bool bAllowStrafe) const override;
		virtual void UpdateMoveFocus() override;
	


	protected:
		/*Map Correlating an obstacle and the angles it was struck from relative to the current flowfields intended direction*/
		FObstacleScan ScannedObstacles;
		FVector PreviousMovement = FVector::ZeroVector;

		const UGridTile* CurrentTile = nullptr;
		const UGridTile* PreviousTile = nullptr;
		const UGridTile* SteeringTile = nullptr;
		bool bIsUsingSteering = false;
		
		int32 SteeringTileAttemptMoves = 0;
		int32 MaxSteeringAttempts = 40;
		TSet<const UGridTile*> RejectedSteeringTiles = TSet<const UGridTile*>();


};
