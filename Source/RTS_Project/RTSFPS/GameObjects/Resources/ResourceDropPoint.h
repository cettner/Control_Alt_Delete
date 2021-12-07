// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TriggerBox.h"
#include "ResourceDropPoint.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API AResourceDropPoint : public ATriggerBox
{
	GENERATED_BODY()

	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;


	AResourceDropPoint();

	// declare overlap begin function
	UFUNCTION()
	virtual void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	// declare overlap end function
	UFUNCTION()
	virtual void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
