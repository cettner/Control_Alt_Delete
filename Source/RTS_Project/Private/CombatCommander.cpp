// Fill out your copyright notice in the Description page of Project Settings.

#include "CombatCommander.h"

ACombatCommander::ACombatCommander()
{

}

void ACombatCommander::BeginPlay()
{
	Super::BeginPlay();
	if(	WManager.Initialize(Cast<ARTSPlayerController>(GetWorld()->GetFirstPlayerController()), GetMesh()))
	{

	}
	
}

bool ACombatCommander::AddWeapon(AWeapon * Added_Weapon)
{
	return(false);
}

void ACombatCommander::SwitchWeapon()
{
}
