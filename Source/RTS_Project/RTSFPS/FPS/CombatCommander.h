// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Commander.h"
#include "Weapons/Weapon.h"
#include "Runtime/Core/Public/Containers/EnumAsByte.h"
#include "CombatCommander.generated.h"




UCLASS()
class RTS_PROJECT_API ACombatCommander : public ACommander
{
	GENERATED_BODY()
	
public:
	ACombatCommander();

public:
	/*Add Weapon to Inventory*/
	void AddWeapon(AWeapon * Added_Weapon);

	/*Remove Weapon From Inventory*/
	void RemoveWeapon(AWeapon* Weapon);

	bool IsWeaponEquipped();

	/*Get Socket Name, TODO:(based on griptype)*/
	virtual FName GetWeaponAttachPoint(AWeapon* Weapon, bool bWantsFirstPerson);

	UFUNCTION(BlueprintCallable)
	virtual EWeaponState GetWeaponState();

	/*Notifier From Weapon that Unequip Animation has Completed*/
	void UnEquipComplete();



protected:
/////////////////////////////////////////////////
//INPUT

	/*Starts Weapon  Selection Process, Selection is done Client Side, with final result being replicated to server*/
	UFUNCTION()
	void SwitchWeaponUp();

	UFUNCTION()
	void SwitchWeaponDown();

	UFUNCTION()
	void OnPrimaryFireStart();

	UFUNCTION()
	void OnPrimaryFireEnd();

	UFUNCTION()
	void OnSecondaryFireStart();

	UFUNCTION()
	void OnSecondaryFireEnd();

	UFUNCTION()
	void OnReload();

	/*Fire Weapon After Character Status Checks Have been Performed*/
	void StartWeaponFire();

	/*Stop Weapon Fire*/
	void StopWeaponFire();

public:
	UFUNCTION(BlueprintCallable)
	TEnumAsByte<Combat_Stance> GetWeaponStance() const;

	/*Read by Animation Blueprint to determine which state machine to use*/
	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<Combat_Stance> Stance = NO_WEAPON_STANCE;

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* ActorInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	virtual void BeginDestroy() override;
	virtual void PostInitializeComponents() override;

protected:
	virtual void OnDeath() override;

protected:
	/** socket or bone name for attaching weapon mesh */
	UPROPERTY(EditDefaultsOnly, Category = Inventory)
	FName WeaponAttachPointFP;

	UPROPERTY(EditDefaultsOnly, Category = Inventory)
	FName WeaponAttachPoint3P;

	/*Inital Array Containing All Starter Weapons*/
	UPROPERTY(EditDefaultsOnly, Category = Inventory)
	TArray<TSubclassOf<AWeapon> > DefaultInventoryClasses;

	/** weapons in inventory */
	UPROPERTY(Transient, Replicated)
	TArray<AWeapon*> Inventory;

protected:
	/** [server] spawns default inventory */
	void SpawnDefaultInventory();

	/** updates current weapon */
	void SetCurrentWeapon(AWeapon* NewWeapon);

	/**
	* [server + local] equips weapon from inventory
	*
	* @param Weapon	Weapon to equip
	*/
	void EquipWeapon(AWeapon* Weapon);

	/**
	* [server + local] equips weapon from inventory
	*
	* @param Weapon	Weapon to equip
	*/
	void UnEquipWeapon();

	/*Set Delay Timer for Selecting Weapon*/
	void SetWeaponEquippedTimer();

	/*Notifer That User has completed Weapon Selection*/
	void WeaponSwitchComplete();

	UFUNCTION(reliable, server, WithValidation)
	void ServerSwitchWeapon(int direction);

	/** equip weapon */
	UFUNCTION(reliable, server, WithValidation)
	void ServerEquipWeapon(AWeapon* NewWeapon);

	/** Unequip weapon */
	UFUNCTION(reliable, server, WithValidation)
	void ServerUnEquipWeapon();

	/*Sets Variables Used in Animation Blueprint*/
	void SetWeaponStance();

protected:
	/** current weapon rep handler */
	UFUNCTION()
	void OnRep_CurrentWeapon(AWeapon* LastWeapon);

	/* if server has begun switching our weapon */
	UFUNCTION()
	void OnRep_bIsSwitchingWeapon();

protected:
	/** currently equipped weapon */
	UPROPERTY(Transient, ReplicatedUsing = OnRep_CurrentWeapon)
	AWeapon* CurrentWeapon;

	/*Weapon selected to be equipped switched by Client during selection process then passed to server*/
	UPROPERTY(Transient)
	AWeapon * NextWeapon;
	
	/*Delay Time for Weapon Selection, Toggling a new weapon will reset countdown*/
	FTimerHandle SwitchWeaponDelayHandler;

	/*Number of seconds Of inactivity before a weapon will begin equipping when cycling weapons*/
	const float SwitchWeaponDelayTime = 2.0;

	/*True If User is currently cycling through available weapons*/
	UPROPERTY(ReplicatedUsing = OnRep_bIsSwitchingWeapon)
	bool bIsSwitching_Weapon = false;

	/*Is a weapon Currently Equipped*/
	bool bIsWeaponEquipped = false;

	/* Current Primary Firing State */
	bool bWantsToFire = false;

	/*Current Secondary Firing State*/
	bool bWantsSecondaryFire = false;

private:
	void HandleSwitchWeapon(int direction);
};
