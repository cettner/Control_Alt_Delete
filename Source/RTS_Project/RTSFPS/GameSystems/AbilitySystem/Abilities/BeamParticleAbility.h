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
	virtual void StartBeam();
	virtual void UpdateBeamParameters();
	virtual bool ShouldSeverBeam() const;
	virtual void SeverBeam();

protected:
	virtual bool IsTargetInRange() const;
	virtual bool IsTargetInLineOfSight() const;

protected:
	virtual void Init(UAbilityComponent * InAbilityComp) override;
	virtual void NotifyOnReady() override;
	virtual void OnAbilityEnd() override;
	virtual void ProcessTarget(AActor * Target) override;

protected:
	virtual void UpdateChannel() override;

protected:
	UPROPERTY(EditDefaultsOnly)
	UNiagaraSystem * BeamEmitter;

	UPROPERTY(EditDefaultsOnly)
	FVector EffectRelativeLocation = FVector(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditDefaultsOnly)
    FRotator EffectRelativeRotation = FRotator(0.0f, 0.0f, 0.0f);

	UPROPERTY(EditDefaultsOnly)
	/*If the Beam Connects to the end target */
	bool bLinkBeamToTarget = false;

	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "bLinkBeamToTarget == true"))
	FName BeamEndParameterName = "User.BeamEnd";

	UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "bLinkBeamToTarget == true"))
	/*The Angle in degrees that the beam will be disconnected from the target if the User's View*/
	float BeamSeveranceAngle = 60.0f;

protected:
	UNiagaraComponent * BeamComp = nullptr;
};

