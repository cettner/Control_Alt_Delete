// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Navigation/PathFollowingComponent.h"
#include "FlockPathFollowingComponent.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UFlockPathFollowingComponent : public UPathFollowingComponent
{
	GENERATED_BODY()

protected:
	
	virtual void OnActorBump(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit) override;


public:
	/* The weight of the Alignment vector component */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Steering Behavior Component")
	float AlignmentWeight;

	/* The weight of the Cohesion vector component */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Steering Behavior Component")
	float CohesionWeight;

	/* The damping of the cohesion force after sum */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Steering Behavior Component")
	float CohesionLerp;

	/* The weight of the Collision vector component */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Steering Behavior Component")
	float CollisionWeight;

	float SeparationLerp;
	float SeparationForce;
	float StimuliLerp;

	/* The weight of the Separation vector component */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Steering Behavior Component")
	float SeparationWeight;

	/* The base movement speed for the Agents */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Steering Behavior Component")
	float BaseMovementSpeed;

	/* The maximum movement speed the Agents can have */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Steering Behavior Component")
	float MaxMovementSpeed;

	/* The maximum radius at which the Agent can detect other Agents */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Steering Behavior Component")
	float VisionRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Steering Behavior Component")
	float CollisionDistanceLook;

	/* Speed to look at direction */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Steering Behavior Component")
	float MaxRotationSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Steering Behavior Component")
	int32 MeshIndex;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI|Steering Behavior Component")
	FTransform Transform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Steering Behavior Component")
	FVector AlignmentComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Steering Behavior Component")
	FVector CohesionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Steering Behavior Component")
	FVector SeparationComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Steering Behavior Component")
	FVector NegativeStimuliComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Steering Behavior Component")
	FVector PositiveStimuliComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Steering Behavior Component")
	float NegativeStimuliMaxFactor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Steering Behavior Component")
	float PositiveStimuliMaxFactor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Steering Behavior Component")
	float InertiaWeigh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Steering Behavior Component")
	float BoidPhysicalRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Steering Behavior Component")
	TArray<AActor*> Neighbourhood;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Steering Behavior Component")
	TArray<AActor*> ActorsInVision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Steering Behavior Component", meta = (Tooltip = "If enable components forces will be visible"))
	bool bEnableDebugDraw;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Steering Behavior Component", meta = (ClampMin = 0.1f, ClampMax = 10.0f))
	float DebugRayDuration;

	const float DefaultNormalizeVectorTolerance = 0.0001f;
	
};
