// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"

#include "MenuSplinePathActor.generated.h"

UCLASS()
class RTS_PROJECT_API AMenuSplinePathActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMenuSplinePathActor();
	USplineComponent * GetSplineComponent(const AActor * InTrackActor = nullptr) const;


protected:
	UPROPERTY(EditAnywhere)
	USplineComponent * SplineComp = nullptr;

};
