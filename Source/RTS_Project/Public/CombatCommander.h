// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Commander.h"
#include "Weapon.h"
#include "WeaponManager.h"
#include "CombatCommander.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum Combat_Stance
{
	NO_WEAPON,
	SWORD_AND_SHIELD,
	TWOHANDED	
};

UCLASS()
class RTS_PROJECT_API ACombatCommander : public ACommander
{
	GENERATED_BODY()
	
public:
	bool AddWeapon(AWeapon * Added_Weapon);
	void SwitchWeapon();
	ACombatCommander();
	virtual void BeginPlay() override;

	Combat_Stance Stance = NO_WEAPON;

private:
	WeaponManager WManager;
};
