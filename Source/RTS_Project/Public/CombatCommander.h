// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Commander.h"
#include "Weapon.h"
#include "WeaponManager.h"
#include "Runtime/Core/Public/Containers/EnumAsByte.h"
#include "CombatCommander.generated.h"

/**
 * 
 */


UCLASS()
class RTS_PROJECT_API ACombatCommander : public ACommander
{
	GENERATED_BODY()
	
public:
	ACombatCommander();
	
	bool AddWeapon(AWeapon * Added_Weapon);
	void SwitchWeaponUp();
	void SwitchWeaponDown();
	
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<Combat_Stance> Stance = NO_WEAPON_STANCE;

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

protected:
	WeaponLoadOut EmptyLoadout;
	WeaponLoadOut S_and_S;

private:
	void SetWeaponStance();
	WeaponManager WManager;
};
