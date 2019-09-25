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

	void SetWeaponEquippedTimer();

	void WeaponSwitchComplete();
	
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadOnly)
	TEnumAsByte<Combat_Stance> Stance = NO_WEAPON_STANCE;

	UPROPERTY(BlueprintReadOnly)
	bool Switch_Weapon = false;

	FName GetWeaponAttatchPoint(AWeapon * Weapon);

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* ActorInputComponent) override;


protected:
	WeaponLoadOut EmptyLoadout;
	WeaponLoadOut S_and_S;

	/** socket or bone name for attaching weapon mesh */
	UPROPERTY(EditDefaultsOnly, Category = Inventory)
	FName WeaponAttachPoint;

	UPROPERTY(EditDefaultsOnly, Category = Inventory)
	TArray<TSubclassOf<AWeapon> > DefaultInventoryClasses;

	/** weapons in inventory */
	UPROPERTY(Transient, Replicated)
	TArray<AWeapon*> Inventory;

	/** currently equipped weapon */
	UPROPERTY(Transient, ReplicatedUsing = OnRep_CurrentWeapon)
	AWeapon* CurrentWeapon;

protected:



	/** current weapon rep handler */
	UFUNCTION()
	void OnRep_CurrentWeapon(AWeapon* LastWeapon);


private:
	void SetWeaponStance();
	WeaponManager WManager;
	FTimerHandle SwitchWeaponDelayHandler;
	const float SwitchWeaponDelayTime = 3.0;
};
