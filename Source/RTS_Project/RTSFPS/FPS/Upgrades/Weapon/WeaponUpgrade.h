// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "../../../GameSystems/UpgradeSystem/Upgrade.h"
#include "../../../FPS/Weapons/Weapon.h"
#include "WeaponUpgrade.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class RTS_PROJECT_API UWeaponUpgrade : public UUpgrade
{
	GENERATED_BODY()
	
public:
	FORCEINLINE const TSubclassOf<AWeapon>& GetTargetWeaponClass() const { return WeaponClass; }

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AWeapon> WeaponClass = nullptr;

};
