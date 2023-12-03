// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AbilityUpgrade.h"
#include "../../../GameSystems/AbilitySystem/Abilities/ProjectileAbility.h"
#include "ProjectileAbilityUpgrade.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UProjectileAbilityUpgrade : public UAbilityUpgrade
{
	GENERATED_BODY()

public:
	virtual bool ApplyUpgrade(UObject* ToUpgrade, const uint32 OldRank, const uint32 NewRank, const bool HasAuthority, const bool IsLocal) const override;
	
public:
	virtual TSubclassOf<UAbility> GetAbilityClass() const override;


protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UProjectileAbility> AbilityClass = nullptr;

	UPROPERTY(EditDefaultsOnly)
	float ProjectileSpeedEnhancement = 10.0f;

	UPROPERTY(EditDefaultsOnly)
	float BaseDamageEnhancement = 10.0f;
};
