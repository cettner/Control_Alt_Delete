// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "RTS_Project/RTSFPS/GameSystems/AbilitySystem/AbilityComponent.h"
#include "AbilityWeapon.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API AAbilityWeapon : public AWeapon
{
	GENERATED_BODY()

		AAbilityWeapon();

public:
	/*Weapon Overrides*/
	/**********************Weapon Overrides***************************/
	virtual void StartFire() override;

	virtual void StopFire() override;

	virtual void StartReload(bool bFromReplication = false) override;

	virtual void StopReload() override;
	/*****************************************************************/

	UFUNCTION(reliable, server, WithValidation)
	void ServerStartUseAbility();

	UFUNCTION(reliable, server, WithValidation)
	void ServerStopUseAbility();

	protected:
		UAbilityComponent * AbilityComp = nullptr;

};
