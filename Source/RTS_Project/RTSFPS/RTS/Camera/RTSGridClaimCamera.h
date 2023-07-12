// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTSGridPlacementCamera.h"
#include "RTSGridClaimCamera.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API ARTSGridClaimCamera : public ARTSGridPlacementCamera
{
	GENERATED_BODY()
	

	protected:
		virtual void PreInitializeGridActor(AGridAttachmentActor* GridActor, const TSubclassOf<AActor> InActorClass, FTransform SpawnTransform = FTransform()) const override;
		virtual void Tick(float Deltatime) override;

	protected:

};
