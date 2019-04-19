// Fill out your copyright notice in the Description page of Project Settings.

#include "CombatCommander.h"

ACombatCommander::ACombatCommander()
{
	EmptyLoadout.PrimaryHand = NO_WEAPON;
	EmptyLoadout.SecondaryHand = NO_WEAPON;
	EmptyLoadout.Stance = NO_WEAPON_STANCE;

	S_and_S.PrimaryHand = ONE_H_AXE_BASIC;
	S_and_S.SecondaryHand = SHIELD;
	S_and_S.Stance = SWORD_AND_SHIELD_STANCE;
}

void ACombatCommander::BeginPlay()
{
	Super::BeginPlay();
	if(	WManager.Initialize(Cast<ARTSPlayerController>(GetWorld()->GetFirstPlayerController()), GetMesh()))
	{
		WManager.AddLoadout(S_and_S);
		WManager.AddLoadout(EmptyLoadout);
		Stance = WManager.GetCurrentLoadOut().Stance;
	}
	
}

void ACombatCommander::SetupPlayerInputComponent(UInputComponent * InputComponent)
{
	InputComponent->BindAction("ScrollUp", IE_Pressed, this, &ACombatCommander::SwitchWeaponUp);
	InputComponent->BindAction("ScrollDown", IE_Pressed, this, &ACombatCommander::SwitchWeaponDown);

	InputComponent->BindAction("ScrollUp", IE_Released, this, &ACombatCommander::SetWeaponEquippedTimer);
	InputComponent->BindAction("ScrollDown", IE_Released, this, &ACombatCommander::SetWeaponEquippedTimer);
}

void ACombatCommander::WeaponSwitchComplete()
{
	Switch_Weapon = false;
}

void ACombatCommander::SetWeaponEquippedTimer()
{
	GetWorldTimerManager().SetTimer(SwitchWeaponDelayHandler, this, &ACombatCommander::WeaponSwitchComplete, 1.0, false, SwitchWeaponDelayTime);
}

bool ACombatCommander::AddWeapon(AWeapon * Added_Weapon)
{
	return(false);
}

void ACombatCommander::SwitchWeaponUp()
{
	Switch_Weapon = true;
	WeaponLoadOut loadout = WManager.GetNextLoadOut();
	Stance = loadout.Stance;
}

void ACombatCommander::SwitchWeaponDown()
{
	Switch_Weapon = true;
	WeaponLoadOut loadout = WManager.GetPreviousLoadOut();
	Stance = loadout.Stance;
}