// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "../../../GameSystems/UpgradeSystem/Upgrade.h"
#include "../../../GameSystems/AbilitySystem/Ability.h"

#include "AbilityUpgrade.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UAbilityUpgrade : public UUpgrade
{
	GENERATED_BODY()
	
	public:
		virtual TSubclassOf<UAbility> GetAbilityClass() const;

	protected:
		virtual bool AdjustUpgradeCost(UAbility* InAbility, const uint32 OldRank, const uint32 NewRank) const;

	protected:
		virtual bool ApplyUpgrade(UObject* ToUpgrade, const uint32 OldRank, const uint32 NewRank, const bool HasAuthority, const bool IsLocal) const override;

	protected:
		UPROPERTY(EditDefaultsOnly)
		FReplicationResourceMap BaseCostReduction = FReplicationResourceMap();
};
