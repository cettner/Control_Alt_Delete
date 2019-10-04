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

#define COLLISION_WEAPON ECC_GameTraceChannel2

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
};

namespace EWeaponState
{
	enum Type
	{
		Idle,
		Firing,
		Reloading,
		Equipping,
		Unequipping,
		Unequipped
	};
}


USTRUCT()
struct FWeaponAnim
{
	GENERATED_USTRUCT_BODY()

	/** animation played on pawn (1st person view) */
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* AnimFirstPerson;

	/** animation played on pawn (3rd person view) */
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	UAnimMontage* AnimThirdPerson;
};

UCLASS(Abstract, Blueprintable)
class RTS_PROJECT_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon(const FObjectInitializer& ObjectInitializer);

public:	
	Weapon_Grip_Type GetType();

public:
//////////////////////////////////////////////
//PURE VIRTUAL FUNCTIONS FOR CHILD CLASSES ///
	virtual void StartFire() PURE_VIRTUAL(AWeapon::StartFire,);

	virtual void StopFire() PURE_VIRTUAL(AWeapon::StopFire,);

	virtual void StartReload(bool bFromReplication = false) PURE_VIRTUAL(AWeapon::StartReload,);

	virtual void StopReload() PURE_VIRTUAL(AWeapon::StopFire,);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assets")
	USkeletalMeshComponent * FirstPersonMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Assets")
	USkeletalMeshComponent * ThirdPersonMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Assets")
	UBoxComponent * CollisionComp;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Assets")
	bool bhassecondary = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Assets")
	FVector SocketLocationOffset = FVector(0,0,0);

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Assets")
	FRotator SocketRotationOffset = FRotator(0,0,0);  

	/*Animation State Machine to Use when holding this Weapon*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Animation")
	TEnumAsByte<Combat_Stance> Stance = NO_WEAPON_STANCE;

public:

	/** weapon is being equipped by owner pawn */
	virtual void OnEquip(const AWeapon* LastWeapon);
	
	/** weapon is now equipped by owner pawn */
	virtual void OnEquipFinished();

	/** weapon is being unequipped by owner pawn */
	virtual void OnUnEquip(const AWeapon* NextWeapon = NULL);

	/** weapon is now unequipped by owner pawn */
	virtual void OnUnEquipFinished();

	/** [server] weapon was added to pawn's inventory */
	virtual void OnEnterInventory(ACombatCommander* NewOwner);

	/** [server] weapon was removed from pawn's inventory */
	virtual void OnLeaveInventory();

	/**Assign Net Owner*/
	void SetOwningPawn(ACombatCommander* NewOwner);

protected:
	/** Attaches weapon mesh to pawn's mesh */
	virtual void AttachMeshToPawn();

	/** Detaches weapon mesh from pawn */
	virtual void DetachMeshFromPawn();

	/** determine current weapon state */
	virtual void DetermineWeaponState();

	/** update weapon state */
	virtual void SetWeaponState(EWeaponState::Type NewState);

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
	EWeaponState::Type CurrentState;

	UPROPERTY(Transient, ReplicatedUsing = OnRep_MyPawn)
	ACombatCommander* MyPawn;


public:

	/** get weapon mesh (needs pawn owner to determine variant) */
	UFUNCTION(BlueprintCallable)
	virtual USkeletalMeshComponent* GetWeaponMesh() const;

	/** get pawn owner */
	UFUNCTION(BlueprintCallable, Category = "Game|Weapon")
	ACombatCommander* GetPawnOwner() const;

	EWeaponState::Type GetCurrentState() const;

protected:
	UFUNCTION()
	void OnRep_MyPawn();

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

private:
	Weapon_Grip_Type Grip_Type = EMPTY_GRIP;
};
