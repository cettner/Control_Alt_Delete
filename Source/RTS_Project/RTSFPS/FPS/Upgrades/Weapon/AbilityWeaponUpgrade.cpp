// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityWeaponUpgrade.h"

TArray<TSubclassOf<UAbility>> UAbilityWeaponUpgrade::GetAbilityOptions() const
{
	if (WeaponClass != nullptr)
	{
		const AAbilityWeapon* weaponcdo = WeaponClass.GetDefaultObject();
	}
	return TArray<TSubclassOf<UAbility>>();
}
