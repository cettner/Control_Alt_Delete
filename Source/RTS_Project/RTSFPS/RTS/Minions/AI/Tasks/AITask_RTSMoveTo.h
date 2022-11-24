// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Navigation/PathFollowingComponent.h"

#include "Tasks/AITask.h"
#include "AITask_RTSMoveTo.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UAITask_RTSMoveTo : public UAITask
{
	GENERATED_BODY()
public:
	UAITask_RTSMoveTo(const FObjectInitializer& ObjectInitializer);
	void SetUp(AAIController* Controller, const FAIMoveRequest& InMoveRequest);

};
