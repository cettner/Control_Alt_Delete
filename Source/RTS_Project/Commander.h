// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTSMinion.h"
#include "Camera/CameraComponent.h"
#include "Commander.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API ACommander : public ARTSMinion
{
	GENERATED_BODY()
	

public:
	ACommander();
	UPROPERTY(EditDefaultsOnly)
	UCameraComponent* FPS_Camera;
	
	
	
};
