// Fill out your copyright notice in the Description page of Project Settings.

#include "CombatCommander.h"
#include "UnrealNetwork.h"
#include "Engine/EngineTypes.h"

ACombatCommander::ACombatCommander()
{
}

void ACombatCommander::BeginPlay()
{
	Super::BeginPlay();
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

void ACombatCommander::AddWeapon(AWeapon * Added_Weapon)
{

}

void ACombatCommander::SwitchWeaponUp()
{
	Switch_Weapon = true;
}

void ACombatCommander::SwitchWeaponDown()
{
	Switch_Weapon = true;
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