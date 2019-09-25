// Fill out your copyright notice in the Description page of Project Settings.

#include "CombatCommander.h"
#include "UnrealNetwork.h"

ACombatCommander::ACombatCommander()
{
	EmptyLoadout.PrimaryHand = NO_WEAPON;
	EmptyLoadout.SecondaryHand = NO_WEAPON;
	EmptyLoadout.Stance = NO_WEAPON_STANCE;

	S_and_S.PrimaryHand = NO_WEAPON;
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
		WManager.SpawnCurrentLoadOut();
	}
	
}

FName ACombatCommander::GetWeaponAttatchPoint(AWeapon * Weapon)
{
	return FName();
}

void ACombatCommander::SetupPlayerInputComponent(UInputComponent * ActorInputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	ActorInputComponent->BindAction("ScrollUp", IE_Pressed, this, &ACombatCommander::SwitchWeaponUp);
	ActorInputComponent->BindAction("ScrollDown", IE_Pressed, this, &ACombatCommander::SwitchWeaponDown);

	ActorInputComponent->BindAction("ScrollUp", IE_Released, this, &ACombatCommander::SetWeaponEquippedTimer);
	ActorInputComponent->BindAction("ScrollDown", IE_Released, this, &ACombatCommander::SetWeaponEquippedTimer);
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


void ACombatCommander::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const

{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// only to local owner: weapon change requests are locally instigated, other clients don't need it
	DOREPLIFETIME_CONDITION(ACombatCommander, Inventory, COND_OwnerOnly);

	// everyone
	DOREPLIFETIME(ACombatCommander, CurrentWeapon);
}

void ACombatCommander::OnRep_CurrentWeapon(AWeapon* LastWeapon)

{
	//SetCurrentWeapon(CurrentWeapon, LastWeapon);
}