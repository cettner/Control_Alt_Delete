// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTS_Project/RTSFPS/GameSystems/AbilitySystem/Abilities/BeamParticleAbility.h"
#include "RTS_Project/RTSFPS/GameSystems/ResourceSystem/Interfaces/ResourceGatherer.h"
#include "MineResourceAbility.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UMineResourceAbility : public UBeamParticleAbility
{
	GENERATED_BODY()
	
	protected:
		UMineResourceAbility();

	public:
		float GetMiningRate() const;
		void SetMiningRate(float InRate);
		uint32 GetMineAmount() const;
		void SetMineAmount(uint32 InAmount);

	protected:
		UFUNCTION()
		virtual void MineResource();
		virtual IResourceGatherer * GetResourceGatherer() const;

	protected:
		virtual void OnAbilityEnd() override;

	protected:
		virtual void StartBeam() override;
		virtual void SeverBeam() override;
		virtual bool ShouldSeverBeam() const override;

	protected:
		virtual bool CanHit(AActor * HitActor) const override;

	protected:
		FTimerHandle MineTickHandler = FTimerHandle();

		UPROPERTY(EditDefaultsOnly)
		float MiningRate = .5f;

		UPROPERTY(EditDefaultsOnly)
		uint32 MineAmount = 4U;
};
