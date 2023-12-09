// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Ability.h"
#include "MeleeAbility.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UMeleeAbility : public UAbility
{
	GENERATED_BODY()
	

	protected:
		virtual void OnTick(float InDeltaseconds) override;
		virtual void OnAbilityEnd() override;

	protected:
		virtual void ProcessHit(const FHitResult& Inblockinghit);

	protected:
		UPROPERTY(EditDefaultsOnly)
		FName MeleeStartTraceSocketName = "MeleeTraceStart";

		UPROPERTY(EditDefaultsOnly)
		FName MeleeEndTraceSocketName = "MeleeTraceEnd";

		UPROPERTY(EditDefaultsOnly)
		uint32 Damage = 10U;

		UPROPERTY(EditDefaultsOnly)
		bool bAllowMultiActorHit = false;

	protected:
		TArray<AActor*> HitActors = TArray<AActor*>();

		bool shouldtrace = true;
};
