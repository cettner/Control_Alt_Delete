// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "../../../GameSystems/UpgradeSystem/Upgrade.h"
#include "../../../FPS/Weapons/AbilityWeapon.h"
#include "AbilityWeaponUpgrade.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class RTS_PROJECT_API UAbilityWeaponUpgrade : public UUpgrade
{
	GENERATED_BODY()
	
public:
	FORCEINLINE const TSubclassOf<AAbilityWeapon>& GetTargetWeaponClass() const { return WeaponClass; }


#if WITH_EDITOR
protected:
	UFUNCTION()
	TArray<TSubclassOf<UAbility>> GetAbilityOptions() const;
#endif

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AAbilityWeapon> WeaponClass = nullptr;

	UPROPERTY(EditDefaultsOnly, meta = (GetOptions = "GetAbilityOptions"))
	TSubclassOf<UAbility> TargetAbility = nullptr;
};
