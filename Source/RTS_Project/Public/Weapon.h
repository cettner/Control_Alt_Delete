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
	DAGGER_GRIP,
	ONE_HANDED_GRIP,
	SPEAR_GRIP,
	SHIELD_GRIP,
	TWO_HANDED_GRIP,
	HALBERD_GRIP
};

namespace EWeaponState
{
	enum Type
	{
		Idle,
		Firing,
		Reloading,
		Equipping,
	};
}

USTRUCT()
struct FWeaponData
{
	GENERATED_USTRUCT_BODY()

		/** inifite ammo for reloads */
		UPROPERTY(EditDefaultsOnly, Category = Ammo)
		bool bInfiniteAmmo;

	/** infinite ammo in clip, no reload required */
	UPROPERTY(EditDefaultsOnly, Category = Ammo)
		bool bInfiniteClip;

	/** max ammo */
	UPROPERTY(EditDefaultsOnly, Category = Ammo)
		int32 MaxAmmo;

	/** clip size */
	UPROPERTY(EditDefaultsOnly, Category = Ammo)
		int32 AmmoPerClip;

	/** initial clips */
	UPROPERTY(EditDefaultsOnly, Category = Ammo)
		int32 InitialClips;

	/** time between two consecutive shots */
	UPROPERTY(EditDefaultsOnly, Category = WeaponStat)
		float TimeBetweenShots;

	/** failsafe reload duration if weapon doesn't have any animation for it */
	UPROPERTY(EditDefaultsOnly, Category = WeaponStat)
		float NoAnimReloadDuration;

	/** defaults */
	FWeaponData()
	{
		bInfiniteAmmo = false;
		bInfiniteClip = false;
		MaxAmmo = 100;
		AmmoPerClip = 20;
		InitialClips = 4;
		TimeBetweenShots = 0.2f;
		NoAnimReloadDuration = 1.0f;
	}
};

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

public:

	/** weapon is being equipped by owner pawn */
	virtual void OnEquip(const AWeapon* LastWeapon);
	
	/** weapon is now equipped by owner pawn */
	virtual void OnEquipFinished();

	/** weapon is holstered by owner pawn */
	virtual void OnUnEquip();

	/** [server] weapon was added to pawn's inventory */
	virtual void OnEnterInventory(ACombatCommander* NewOwner);

	/** [server] weapon was removed from pawn's inventory */
	virtual void OnLeaveInventory();

protected:
	/** Attaches weapon mesh to pawn's mesh */
	void AttachMeshToPawn();

	/** Detaches weapon mesh from pawn */
	void DetachMeshFromPawn();

	/** determine current weapon state */
	void DetermineWeaponState();

	/** update weapon state */
	void SetWeaponState(EWeaponState::Type NewState);

	/* Helper Functions */
	UAudioComponent* PlayWeaponSound(USoundCue* Sound);
	float PlayWeaponAnimation(const FWeaponAnim& Animation);
	void StopWeaponAnimation(const FWeaponAnim& Animation);

protected:
	/** equip animations */
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	FWeaponAnim EquipAnim;

	/** fire animations */
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	FWeaponAnim FireAnim;

	/** last time when this weapon was switched to */
	float EquipStartedTime;

	/** how much time weapon needs to be equipped */
	float EquipDuration;
	
	/** Handle for efficient management of OnEquipFinished timer */
	FTimerHandle TimerHandle_OnEquipFinished;

	/** equip sound */
	UPROPERTY(EditDefaultsOnly, Category = Sound)
	USoundCue* EquipSound;

	/** is equip animation playing? */
	bool bPendingEquip = false;

	/** is weapon currently equipped? */
	bool bIsEquipped = false;

	/** current weapon state */
	EWeaponState::Type CurrentState;

	UPROPERTY(Transient, ReplicatedUsing = OnRep_MyPawn)
	ACombatCommander* MyPawn;


public:

	/** get weapon mesh (needs pawn owner to determine variant) */
	UFUNCTION(BlueprintCallable)
	USkeletalMeshComponent* GetWeaponMesh() const;

	/** get pawn owner */
	UFUNCTION(BlueprintCallable, Category = "Game|Weapon")
	ACombatCommander* GetPawnOwner() const;

protected:
	UFUNCTION()
	void OnRep_MyPawn();

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const;

private:
	Weapon_Grip_Type Grip_Type = EMPTY_GRIP;
};
