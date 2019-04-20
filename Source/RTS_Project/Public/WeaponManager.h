// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Weapon.h"
#include "RTSSpawnHelper.h"
#include "RTSPlayerController.h"
#include "CoreMinimal.h"

/**
 * 
 */
UENUM(BlueprintType)
enum Combat_Stance
{
	NO_WEAPON_STANCE,
	SWORD_AND_SHIELD_STANCE,
	TWO_HANDED_STANCE	
};

struct WeaponLoadOut
{
	Weapon_Types PrimaryHand = NO_WEAPON;
	Weapon_Types SecondaryHand = NO_WEAPON;
	Combat_Stance Stance = NO_WEAPON_STANCE;
};



class RTS_PROJECT_API WeaponManager
{
public:
	WeaponManager();
	~WeaponManager();
	bool AddLoadout(WeaponLoadOut Added_Set);
	bool Initialize(ARTSPlayerController * Controller, USkeletalMeshComponent * character_mesh);
	bool SpawnCurrentLoadOut();
	bool DeSpawnCurrentLoadout();

	WeaponLoadOut GetCurrentLoadOut();
	WeaponLoadOut GetNextLoadOut();
	WeaponLoadOut GetPreviousLoadOut();

	AWeapon * GetPrimaryWeapon(){return(PrimaryHand);}
	AWeapon * GetSecondaryWeapon(){return(SecondaryHand);}

private:
	UPROPERTY(transient)
	AWeapon * PrimaryHand;

	UPROPERTY(transient)
	AWeapon * SecondaryHand;

	static const int max_inventory_size = 3;

	int loadout_index;

	TArray<WeaponLoadOut> Arsenal;

	ARTSPlayerController * PC;

	USkeletalMeshComponent * Character;	

	bool initialized = false;
};
