// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Ability.h"
#include "TraceTargetAbility.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UTraceTargetAbility : public UAbility
{
	GENERATED_BODY()

public:
	virtual void OnEffect() override;
	virtual void OnTick(float DeltaSeconds) override;
	virtual void NotifyOnReady() override;
	virtual void OnAbilityEnd() override;


protected:
	UFUNCTION()
	virtual void UpdateChannel();


protected:
	FTimerHandle ChannelPulseHandler = FTimerHandle();

	UPROPERTY(EditDefaultsOnly)
	/*If True the channel will trigger its effect immediatly on channel start*/
	bool bUpdatesOnReady = true;

	UPROPERTY(EditDefaultsOnly)
	float PulseRate = .5f;

	UPROPERTY(EditDefaultsOnly)
	float Range = 100.0f;
};
