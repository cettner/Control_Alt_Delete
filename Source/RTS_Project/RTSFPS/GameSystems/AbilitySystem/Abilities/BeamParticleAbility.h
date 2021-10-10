// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TraceTargetAbility.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "BeamParticleAbility.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UBeamParticleAbility : public UTraceTargetAbility
{
	GENERATED_BODY()
	
protected:
	UBeamParticleAbility();
	virtual void Init(UAbilityComponent * InAbilityComp) override;
	virtual void NotifyOnReady() override;
	virtual void OnAbilityEnd() override;

protected:
	UPROPERTY(EditDefaultsOnly)
	UNiagaraSystem * BeamEmitter;

	UPROPERTY(EditDefaultsOnly)
	FVector EffectRelativeLocation = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditDefaultsOnly)
    FRotator EffectRelativeRotation = FRotator(0.0f, 0.0f, 0.0f);

protected:
	UNiagaraComponent * BeamComp = nullptr;
};

