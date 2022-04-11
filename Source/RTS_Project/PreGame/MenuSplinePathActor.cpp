// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuSplinePathActor.h"

// Sets default values
AMenuSplinePathActor::AMenuSplinePathActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootTransform"));
	SplineComp = CreateDefaultSubobject<USplineComponent>(TEXT("RootSpline"));
	SplineComp->SetupAttachment(RootComponent);

}

USplineComponent * AMenuSplinePathActor::GetSplineComponent(const AActor * InTrackActor) const
{
	return SplineComp;
}

