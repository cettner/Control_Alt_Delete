// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "Camera/CameraActor.h"
#include "Curves/CurveFloat.h"
#include "Components/StaticMeshComponent.h"
#include "Components/TimelineComponent.h"

#include "MenuSplinePathActor.h"
#include "MenuPawn.generated.h"

UCLASS()
class RTS_PROJECT_API AMenuPawn : public ACameraActor
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMenuPawn();
	virtual bool GetIsTrackEnabled() const;
	virtual void SetIsTrackEnabled(bool InTrackEnable);

protected:
	UFUNCTION()
	virtual void UpdateTrackPosition();

	UFUNCTION()
	virtual void OnTrackCompleted();

protected:
	virtual void PostInitializeComponents() override;
	virtual void Tick(float InDeltaTime) override;


protected:
	UPROPERTY(EditDefaultsOnly)
	bool bEnabledAtStart = true;

	UPROPERTY(EditDefaultsOnly)
	float DurationofTrack = 1.0f;
	
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float PercentTrackStartOffset = 0.0f;

	UPROPERTY(EditDefaultsOnly)
	UCurveFloat * TrackAlphaCurve;

	UPROPERTY(EditInstanceOnly)
	AMenuSplinePathActor * CurrentSplineActor = nullptr;

protected:

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent * DebugMesh = nullptr;


	/*Runtime Variables*/
protected:
	bool bIsTrackEnabled = false;

	FTimeline MyTimeline;
	FOnTimelineFloat TimelineCallback;
	FOnTimelineEventStatic TimelineFinishedCallback;

};
