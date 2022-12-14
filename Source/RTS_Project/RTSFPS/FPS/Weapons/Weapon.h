// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "Animation/AnimMontage.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "Weapon.generated.h"


/*Forward Declarations*/
class ACombatCommander;



/*Specifies Attatchment Location on Mesh*/
UENUM(BlueprintType)
enum Weapon_Grip_Type
{
	EMPTY_GRIP,
	RIFLE_GRIP,
};

/**
 *
 */
UENUM(BlueprintType)
enum Combat_Stance
{
	NO_WEAPON_STANCE,
	MAGE_STANCE,
	RIFLE_STANCE,
};


UENUM(BlueprintType)
enum EWeaponState
{
	Idle,
	Firing,
	Reloading,
	Equipping,
	Unequipping,
	Unequipped
};


USTRUCT()
struct FWeaponAnim
{
	GENERATED_USTRUCT_BODY()

	/** animation played on pawn (1st person view) */
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* AnimFirstPerson = nullptr;

	/** animation played on pawn (3rd person view) */
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* AnimThirdPerson = nullptr;
};

UCLASS(Abstract, Blueprintable)
class RTS_PROJECT_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

public:	
	Weapon_Grip_Type GetType();

public:
//////////////////////////////////////////////
//PURE VIRTUAL FUNCTIONS FOR CHILD CLASSES ///
	virtual void StartFire() PURE_VIRTUAL(AWeapon::StartFire,);

	virtual void StopFire() PURE_VIRTUAL(AWeapon::StopFire,);

	virtual void StartReload(bool bFromReplication = false) PURE_VIRTUAL(AWeapon::StartReload,);

	virtual void StopReload() PURE_VIRTUAL(AWeapon::StopFire,);
/**********************************************************************************************/
	virtual void StartSecondaryFire();

	virtual void StopSecondaryFire();

	virtual void WeaponPrimarySetting(int ModeToggle);
	
	virtual void WeaponSecondarySetting(int ModeToggle);
/*********************************************************************************************/

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Meshes")
	USkeletalMeshComponent * FirstPersonMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Meshes")
	USkeletalMeshComponent * ThirdPersonMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Meshes")
	UBoxComponent * CollisionComp;

public:
	/*If True Will bind secondary fire button on equipped*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Gameplay")
	bool bHasSecondary = false;  

	/*If true, Secondary will be allowed to fire while primary fire is active*/
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay", meta = (EditCondition = "bHasSecondary"))
	bool bAllowSimultaneousFire = false;

	/*Animation State Machine to Use when holding this Weapon*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
	TEnumAsByte<Combat_Stance> Stance = NO_WEAPON_STANCE;

public:

	/** weapon is being equipped by owner pawn */
	virtual void OnEquip();
	
	/** weapon is now equipped by owner pawn */
	virtual void OnEquipFinished();

	/** weapon is being unequipped by owner pawn */
	virtual void OnUnEquip();

	/** weapon is now unequipped by owner pawn */
	virtual void OnUnEquipFinished();

	/** [server] weapon was added to pawn's inventory */
	virtual void OnEnterInventory(ACombatCommander* NewOwner);

	/** [server] weapon was removed from pawn's inventory */
	virtual void OnLeaveInventory();

	/**Assign Net Owner*/
	virtual void SetOwningPawn(ACombatCommander* NewOwner);

	/*On Clients, the Current Weapon may replicate before the pawn does, this allows the ability to check if Pawn Replication is still in progress*/
	virtual bool HasPawnReplicated() const;

	/*Returns True if the Weapon is currently equipped by the players local Pawn*/
	virtual bool IsLocallyEquipped() const;

protected:
	/** Attaches weapon mesh to pawn's mesh */
	virtual void AttachMeshToPawn();

	/** Detaches weapon mesh from pawn */
	virtual void DetachMeshFromPawn();

	/** determine current weapon state */
	virtual void DetermineWeaponState();

	/** update weapon state */
	virtual void SetWeaponState(EWeaponState NewState);

	/*Determine Attatchment State*/
	bool IsAttachedToPawn() const;

	/* Helper Functions */
	UAudioComponent* PlayWeaponSound(USoundCue* Sound);
	float PlayWeaponAnimation(const FWeaponAnim& Animation);
	float GetAnimationTime(const FWeaponAnim& Animation);
	void StopWeaponAnimation(const FWeaponAnim& Animation);

	/** Get the aim of the weapon, allowing for adjustments to be made by the weapon */
	virtual FVector GetAdjustedAim() const;

protected:
	/** equip animations */
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	FWeaponAnim EquipAnim;

	/** unequip animations */
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	FWeaponAnim UnEquipAnim;
	
	/** Handle for efficient management of OnEquipFinished timer */
	FTimerHandle TimerHandle_OnEquipFinished;

	/** Handle for efficient management of OnUnEquipFinished timer */
	FTimerHandle TimerHandle_OnUnEquipFinished;

	/** equip sound */
	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundCue* EquipSound;

	/** is equip animation playing? */
	bool bPendingEquip = false;

	/**is unequip animation playing?*/
	bool bPendingUnEquip = false;

	/** is weapon currently equipped? */
	bool bIsEquipped = false;

	/** current weapon state */
	EWeaponState CurrentState;

	UPROPERTY(Transient, ReplicatedUsing = OnRep_MyPawn)
	ACombatCommander* MyPawn = nullptr;


public:

	/** get weapon mesh (needs pawn owner to determine variant) */
	virtual USkeletalMeshComponent* GetWeaponMesh() const;

	/** get pawn owner */
	ACombatCommander* GetPawnOwner() const;

	EWeaponState GetCurrentState() const;

protected:
	UFUNCTION()
	virtual void OnRep_MyPawn();

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

private:
	Weapon_Grip_Type Grip_Type = EMPTY_GRIP;
};
