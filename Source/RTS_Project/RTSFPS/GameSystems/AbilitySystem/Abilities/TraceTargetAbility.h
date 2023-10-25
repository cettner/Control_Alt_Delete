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
	virtual void NotifyOnReady() override;
	virtual void OnAbilityEnd() override;
	virtual void ProcessTraceHit(FHitResult HitResult, FVector StartTrace, FVector EndTrace);
	

protected:
	UFUNCTION()
	virtual void UpdateChannel();
	
	void SetAbilityTarget(AActor* NewTarget, FHitResult AssociatedHit = FHitResult());
	FORCEINLINE AActor* GetAbilityTarget() const { return AbilityTarget; };
	template < class T >
	T* GetAbilityTarget() const
	{
		return Cast<T>(GetAbilityTarget());
	}

	bool GetHitInfoFor(AActor* HitTarget, FHitResult& Hit) const;

	virtual bool CanHit(AActor * HitActor) const;

	UFUNCTION()
	void OnRep_AbilityTarget();

protected:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

protected:
	FTimerHandle ChannelPulseHandler = FTimerHandle();

	UPROPERTY(EditDefaultsOnly)
	/*If True the channel will trigger its effect immediatly on channel start*/
	bool bUpdatesOnReady = true;
	
	/*If True ProcessTraceHit Will be called only when the trace hits a target, otherwise is always called*/
	UPROPERTY(EditDefaultsOnly)
	bool bOnlyProcessTrueHits = true;

	UPROPERTY(EditDefaultsOnly)
	float PulseRate = .25f;

	UPROPERTY(EditDefaultsOnly)
	float Range = 150.0f;

	/*Runtime*/
protected:
	UPROPERTY(ReplicatedUsing = OnRep_AbilityTarget)
	AActor* AbilityTarget = nullptr;

	FHitResult LastAbilityHit = FHitResult();
};
