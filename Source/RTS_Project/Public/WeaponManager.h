// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Weapon.h"
#include "RTSSpawnHelper.h"
#include "RTSPlayerController.h"
#include "CoreMinimal.h"

/**
 * 
 */

struct WeaponLoadOut
{
	Weapon_Types PrimaryHand = EMPTY;
	Weapon_Types SecondaryHand = EMPTY;
};

class RTS_PROJECT_API WeaponManager
{
public:
	WeaponManager();
	~WeaponManager();
	void AddLoadout(WeaponLoadOut Added_Set);
	bool Initialize(ARTSPlayerController * Controller, USkeletalMeshComponent * character_mesh);

private:
	UPROPERTY(transient)
	AWeapon * PrimaryHand;

	UPROPERTY(transient)
	AWeapon * SecondaryHand;

	static const int inventory_size = 3;

	int loadout_index = 0;

	TArray<WeaponLoadOut> Arsenal;

	ARTSPlayerController * PC;

	USkeletalMeshComponent * Character;	

	bool initialized = false;
};
