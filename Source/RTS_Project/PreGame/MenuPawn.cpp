// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuPawn.h"

// Sets default values
AMenuPawn::AMenuPawn() : Super()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	DebugMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DebugMesh"));
	DebugMesh->SetupAttachment(RootComponent);
}

bool AMenuPawn::GetIsTrackEnabled() const
{
	return bIsTrackEnabled;
}

void AMenuPawn::SetIsTrackEnabled(bool InTrackEnable)
{
	bIsTrackEnabled = InTrackEnable;

	if (bIsTrackEnabled == true && !MyTimeline.IsPlaying())
	{
		const float totalplayduration = 1.0f / DurationofTrack;
		MyTimeline.SetPlayRate(totalplayduration);
		MyTimeline.SetNewTime(PercentTrackStartOffset);
		MyTimeline.SetLooping(true);
		MyTimeline.PlayFromStart();
	}

}

void AMenuPawn::UpdateTrackPosition()
{
	if (IsValid(CurrentSplineActor))
	{

		const USplineComponent * splinecomp = CurrentSplineActor->GetSplineComponent(this);

		if (IsValid(splinecomp))
		{
			const float timelineindex = MyTimeline.GetPlaybackPosition();
			const float trackalpha = TrackAlphaCurve->GetFloatValue(timelineindex);
			const float splinelength = splinecomp->GetSplineLength();

			const float currenttrackposition = FMath::Lerp(0.0f, splinelength, trackalpha);


			const FVector newlocation = splinecomp->GetLocationAtDistanceAlongSpline(currenttrackposition, ESplineCoordinateSpace::World);
			const FRotator newrotation = splinecomp->GetRotationAtDistanceAlongSpline(currenttrackposition, ESplineCoordinateSpace::World);
			SetActorLocationAndRotation(newlocation, newrotation);
		}
	}



}

void AMenuPawn::OnTrackCompleted()
{
}

void AMenuPawn::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (IsValid(TrackAlphaCurve))
	{
		TimelineCallback.BindUFunction(this, FName("UpdateTrackPosition"));
		TimelineFinishedCallback.BindUFunction(this, FName("OnTrackCompleted"));

		MyTimeline.AddInterpFloat(TrackAlphaCurve, TimelineCallback);
		MyTimeline.SetTimelineFinishedFunc(TimelineFinishedCallback);
	}



	SetIsTrackEnabled(bEnabledAtStart);

}

void AMenuPawn::Tick(float InDeltaTime)
{
	Super::Tick(InDeltaTime);
	MyTimeline.TickTimeline(InDeltaTime);
}
