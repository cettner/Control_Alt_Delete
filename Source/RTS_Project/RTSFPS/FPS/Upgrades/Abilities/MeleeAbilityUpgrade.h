// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityUpgrade.h"

#include "../../../GameSystems/AbilitySystem/Abilities/MeleeAbility.h"
#include "MeleeAbilityUpgrade.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UMeleeAbilityUpgrade : public UAbilityUpgrade
{
	GENERATED_BODY()

	public:
		virtual bool ApplyUpgrade(UObject* ToUpgrade, const uint32 OldRank, const uint32 NewRank, const bool HasAuthority, const bool IsLocal) const override;

	public:
		virtual TSubclassOf<UAbility> GetAbilityClass() const override;


	protected:
		UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UMeleeAbility> AbilityClass = nullptr;

		UPROPERTY(EditDefaultsOnly)
		uint32 DamageEnhancement = 10U;

		UPROPERTY(EditDefaultsOnly)
		uint32 MultiHitEnhancement = 1U;


};
