// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Commander.h"
#include "Weapon.h"
#include "WeaponManager.h"
#include "Runtime/Core/Public/Containers/EnumAsByte.h"
#include "CombatCommander.generated.h"




UCLASS()
class RTS_PROJECT_API ACombatCommander : public ACommander
{
	GENERATED_BODY()
	
public:
	ACombatCommander();

	virtual void BeginPlay() override;

	
	void AddWeapon(AWeapon * Added_Weapon);

	void RemoveWeapon(AWeapon* Weapon);

	void SwitchWeaponUp();

	void SwitchWeaponDown();

	void SetWeaponEquippedTimer();

	void WeaponSwitchComplete();
	
	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<Combat_Stance> Stance = NO_WEAPON_STANCE;

	UPROPERTY(BlueprintReadOnly)
	bool Switch_Weapon = false;

	FName GetWeaponAttachPoint(AWeapon * Weapon);

	void SetWeaponStance();

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* ActorInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	virtual void PostInitializeComponents() override;

protected:
	/** socket or bone name for attaching weapon mesh */
	UPROPERTY(EditDefaultsOnly, Category = Inventory)
	FName WeaponAttachPoint;

	UPROPERTY(EditDefaultsOnly, Category = Inventory)
	TArray<TSubclassOf<AWeapon> > DefaultInventoryClasses;

	/** weapons in inventory */
	UPROPERTY(Transient, Replicated)
	TArray<AWeapon*> Inventory;



protected:
	/** [server] spawns default inventory */
	void SpawnDefaultInventory();

	/** updates current weapon */
	void SetCurrentWeapon(AWeapon* NewWeapon, AWeapon* LastWeapon = NULL);

	/**
	* [server + local] equips weapon from inventory
	*
	* @param Weapon	Weapon to equip
	*/
	void EquipWeapon(AWeapon* Weapon);

	/** equip weapon */
	UFUNCTION(reliable, server, WithValidation)
	void ServerEquipWeapon(AWeapon* NewWeapon);

protected:
	/** current weapon rep handler */
	UFUNCTION()
	void OnRep_CurrentWeapon(AWeapon* LastWeapon);


private:
	/** currently equipped weapon */
	UPROPERTY(Transient, ReplicatedUsing = OnRep_CurrentWeapon)
	AWeapon* CurrentWeapon;

	FTimerHandle SwitchWeaponDelayHandler;
	const float SwitchWeaponDelayTime = 3.0;
};
