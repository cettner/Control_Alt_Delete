// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTSMinion.h"
#include "RTSCatapult.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API ARTSCatapult : public ARTSMinion
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditDefaultsOnly,Category = Debug)
	float LaunchTime = 5.0;
private:
	FTimerHandle Launch_Handler;
	
	
};
