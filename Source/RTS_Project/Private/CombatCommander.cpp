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

FName ACombatCommander::GetWeaponAttachPoint(AWeapon * Weapon)
{
	return(WeaponAttachPoint);
}

void ACombatCommander::SetWeaponStance()
{
	if (CurrentWeapon && CurrentWeapon->GetCurrentState() == EWeaponState::Idle)
	{
		Stance = CurrentWeapon->Stance;
	}
	else
	{
	    Stance = NO_WEAPON_STANCE;
	}
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
	EquipWeapon(NextWeapon);
	SetWeaponStance();
	NextWeapon = NULL;

}

void ACombatCommander::SetWeaponEquippedTimer()
{
	GetWorldTimerManager().SetTimer(SwitchWeaponDelayHandler, this, &ACombatCommander::WeaponSwitchComplete, 1.0, false, SwitchWeaponDelayTime);
}

void ACombatCommander::AddWeapon(AWeapon * Added_Weapon)
{

	if (Added_Weapon && Role == ROLE_Authority)
	{

		Added_Weapon->OnEnterInventory(this);

		Inventory.AddUnique(Added_Weapon);

	}
}

void ACombatCommander::EquipWeapon(AWeapon* Weapon)
{
	if (Weapon)
	{
		if (Role == ROLE_Authority)
		{
			SetCurrentWeapon(Weapon, CurrentWeapon);
		}
		else
		{
			ServerEquipWeapon(Weapon);
		}
	}
}

void ACombatCommander::RemoveWeapon(AWeapon* Weapon)
{
	if (Weapon && Role == ROLE_Authority)
	{
		Weapon->OnLeaveInventory();
		Inventory.RemoveSingle(Weapon);
	}
}

bool ACombatCommander::ServerEquipWeapon_Validate(AWeapon* Weapon)
{
	return(true);
}

void ACombatCommander::ServerEquipWeapon_Implementation(AWeapon* Weapon)
{
	EquipWeapon(Weapon);
}

void ACombatCommander::SwitchWeaponUp()
{
	Switch_Weapon = true;
	if (Inventory.Num() >= 2)
	{
		const int32 CurrentWeaponIdx = Inventory.IndexOfByKey(CurrentWeapon);
		NextWeapon = Inventory[(CurrentWeaponIdx + 1) % Inventory.Num()];
		CurrentWeapon->OnUnEquip();
		SetWeaponStance();
	}
}

void ACombatCommander::SwitchWeaponDown()
{
	Switch_Weapon = true;
	if (Inventory.Num() >= 2)
	{
		const int32 CurrentWeaponIdx = Inventory.IndexOfByKey(CurrentWeapon);
		NextWeapon = Inventory[(CurrentWeaponIdx - 1 + Inventory.Num()) % Inventory.Num()];
		CurrentWeapon->OnUnEquip();
		SetWeaponStance();
	}
}

void ACombatCommander::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	// only to local owner: weapon change requests are locally instigated, other clients don't need it
	DOREPLIFETIME_CONDITION(ACombatCommander, Inventory, COND_OwnerOnly);

	// everyone
	DOREPLIFETIME(ACombatCommander, CurrentWeapon);
}

void ACombatCommander::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SpawnDefaultInventory();
}

void ACombatCommander::SpawnDefaultInventory()
{
	if (Role < ROLE_Authority)
	{
		return;
	}

	int32 NumWeaponClasses = DefaultInventoryClasses.Num();
	for (int32 i = 0; i < NumWeaponClasses; i++)
	{
		if (DefaultInventoryClasses[i])
		{
			FActorSpawnParameters SpawnInfo;
			SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			AWeapon* NewWeapon = GetWorld()->SpawnActor<AWeapon>(DefaultInventoryClasses[i], SpawnInfo);
			AddWeapon(NewWeapon);
		}
	}

	// equip first weapon in inventory
	if (Inventory.Num() > 0)
	{
		EquipWeapon(Inventory[0]);
	}
}

void ACombatCommander::SetCurrentWeapon(AWeapon * NewWeapon, AWeapon * LastWeapon)
{

	AWeapon* LocalLastWeapon = NULL;


	if (LastWeapon != NULL)
	{
		LocalLastWeapon = LastWeapon;
	}
	else if (NewWeapon != CurrentWeapon)
	{
		LocalLastWeapon = CurrentWeapon;
	}

	// unequip previous

	if (LocalLastWeapon)
	{
		LocalLastWeapon->OnUnEquip();
	}

	CurrentWeapon = NewWeapon;

	// equip new one

	if (NewWeapon)
	{
		NewWeapon->SetOwningPawn(this);	// Make sure weapon's MyPawn is pointing back to us. During replication, we can't guarantee APawn::CurrentWeapon will rep after AWeapon::MyPawn!

		NewWeapon->OnEquip(LastWeapon);
	}

	// Change How we are holding the weapon in animation blueprint.
	SetWeaponStance();
}

void ACombatCommander::OnRep_CurrentWeapon(AWeapon* LastWeapon)
{
	SetCurrentWeapon(CurrentWeapon, LastWeapon);
	SetWeaponStance();
}