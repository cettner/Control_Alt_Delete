// Fill out your copyright notice in the Description page of Project Settings.

#include "CombatCommander.h"
#include "UnrealNetwork.h"
#include "Engine/EngineTypes.h"
#include "Components/CapsuleComponent.h"

ACombatCommander::ACombatCommander()
{
	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(COLLISION_WEAPON, ECR_Block);
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
	if (CurrentWeapon && CurrentWeapon->GetCurrentState() < EWeaponState::Unequipping)
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

	ActorInputComponent->BindAction("LeftMouse", IE_Pressed, this, &ACombatCommander::OnPrimaryFireStart);
	ActorInputComponent->BindAction("LeftMouse", IE_Released, this, &ACombatCommander::OnPrimaryFireEnd);

	ActorInputComponent->BindAction("RightMouse", IE_Pressed, this, &ACombatCommander::OnSecondaryFireStart);
	ActorInputComponent->BindAction("RightMouse", IE_Released, this, &ACombatCommander::OnSecondaryFireEnd);

	ActorInputComponent->BindAction("RKey", IE_Pressed, this, &ACombatCommander::OnReload);
}

void ACombatCommander::WeaponSwitchComplete()
{
	bIsSwitching_Weapon = false;
	if (!bIsWeaponEquipped)
	{
		EquipWeapon(NextWeapon);
		//Replication wont occur if we switched to our old weapon since nothing actually changed, so we kick off equipped locally
		if (CurrentWeapon == NextWeapon && Role != ROLE_Authority)
		{
			OnRep_CurrentWeapon(CurrentWeapon);
		}

		NextWeapon = nullptr;
	}
}

void ACombatCommander::HandleSwitchWeapon(int direction)
{
	if (Inventory.Num() >= 2)
	{
		/*If we're already Cycling Weapons */
		if (NextWeapon && bIsSwitching_Weapon)
		{
			const int32 CurrentWeaponIdx = Inventory.IndexOfByKey(NextWeapon);
			NextWeapon = Inventory[(CurrentWeaponIdx + direction + Inventory.Num()) % Inventory.Num()];
		}
		else if (CurrentWeapon) // Get the nextweapon based on the current
		{
			const int32 CurrentWeaponIdx = Inventory.IndexOfByKey(CurrentWeapon);
			NextWeapon = Inventory[(CurrentWeaponIdx + direction + Inventory.Num()) % Inventory.Num()];
		}

		if (bIsSwitching_Weapon)
		{
			SetWeaponEquippedTimer();
		}
		else if (CurrentWeapon && CurrentWeapon->GetCurrentState() == EWeaponState::Idle)
		{
			bIsSwitching_Weapon = true;
			UnEquipWeapon();
			SetWeaponEquippedTimer();
		}
	}
}

void ACombatCommander::SetWeaponEquippedTimer()
{
	GetWorldTimerManager().SetTimer(SwitchWeaponDelayHandler, this, &ACombatCommander::WeaponSwitchComplete, SwitchWeaponDelayTime, false);
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

void ACombatCommander::UnEquipWeapon()
{
	CurrentWeapon->OnUnEquip();
}

void ACombatCommander::UnEquipComplete()
{
	SetWeaponStance();
	bIsWeaponEquipped = false;
	if (!bIsSwitching_Weapon)
	{
		WeaponSwitchComplete();
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


bool ACombatCommander::ServerUnEquipWeapon_Validate()
{
	return(true);
}

void ACombatCommander::ServerUnEquipWeapon_Implementation()
{
	UnEquipWeapon();
}

////////////////////////////////////////////////////////////////////////////////
//INPUT
void ACombatCommander::SwitchWeaponUp()
{
	HandleSwitchWeapon(1);
}

void ACombatCommander::SwitchWeaponDown()
{
	HandleSwitchWeapon(-1);
}

void ACombatCommander::OnPrimaryFireStart()
{
	StartWeaponFire();
}

void ACombatCommander::OnPrimaryFireEnd()
{
	StopWeaponFire();
}

void ACombatCommander::OnSecondaryFireStart()
{
}

void ACombatCommander::OnSecondaryFireEnd()
{
}

void ACombatCommander::OnReload()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StartReload();
	}
}

void ACombatCommander::StartWeaponFire()
{
	if (!bWantsToFire)
	{
		bWantsToFire = true;
		if (CurrentWeapon)
		{
			CurrentWeapon->StartFire();
		}
	}
}

void ACombatCommander::StopWeaponFire()
{
	if (bWantsToFire)
	{
		bWantsToFire = false;
		if (CurrentWeapon)
		{
			CurrentWeapon->StopFire();
		}
	}
}

////////////////////////////////////////////////////////////////////////////////
//Replication

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
	bIsWeaponEquipped = true;
}


