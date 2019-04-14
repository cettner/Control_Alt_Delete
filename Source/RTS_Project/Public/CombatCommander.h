// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Commander.h"
#include "Weapon.h"
#include "CombatCommander.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API ACombatCommander : public ACommander
{
	GENERATED_BODY()
	
public:
	bool AddWeapon(AWeapon * Added_Weapon);
	void SwitchWeapon();
	
};
