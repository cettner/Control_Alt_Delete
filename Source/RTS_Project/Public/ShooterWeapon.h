// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "UnrealNetwork.h"
#include "ShooterWeapon.generated.h"

/**
 * 
 */
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

UCLASS()
class RTS_PROJECT_API AShooterWeapon : public AWeapon
{
	GENERATED_BODY()
	

		//////////////////////////////////////////////////////////////////////////
		// Ammo
public:

	enum class EAmmoType
	{
		EBullet,
		EProjectile,
		EMax,
	};

public:
	AShooterWeapon(const FObjectInitializer& ObjectInitializer);

/////////////// Input
	
public:
	/** [local + server] start weapon fire */
	virtual void StartFire() override;

	/** [local + server] stop weapon fire */
	virtual void StopFire() override;

	/** [all] start weapon reload */
	virtual void StartReload(bool bFromReplication = false) override;

	/** [local + server] interrupt weapon reload */
	virtual void StopReload() override;

	/** [server] performs actual reload */
	virtual void ReloadWeapon();

	/** trigger reload from server */
	UFUNCTION(reliable, client)
	void ClientStartReload();

protected:
//////////////////////////////////////////////////////////////////////////
// Input - server side

	UFUNCTION(reliable, server, WithValidation)
	void ServerStartFire();

	UFUNCTION(reliable, server, WithValidation)
	void ServerStopFire();

	UFUNCTION(reliable, server, WithValidation)
	void ServerStartReload();

	UFUNCTION(reliable, server, WithValidation)
	void ServerStopReload();

protected:
//////////////////////////////////////////////////////////////////////////
// Weapon usage

/** [local] weapon specific fire implementation */
	virtual void FireWeapon() PURE_VIRTUAL(AShooterWeapon::FireWeapon,);

	/** [server] fire & update ammo */
	UFUNCTION(reliable, server, WithValidation)
	void ServerHandleFiring();

	virtual EAmmoType GetAmmoType() const;

	void UseAmmo();

	/** [server] add ammo */
	void GiveAmmo(int AddAmount);

	/** [local + server] handle weapon fire */
	void HandleFiring();

	/** [local + server] firing started */
	virtual void OnBurstStarted();

	/** [local + server] firing finished */
	virtual void OnBurstFinished();

	/** Called in network play to do the cosmetic fx for firing */
	virtual void SimulateWeaponFire();

	/** Called in network play to stop cosmetic fx (e.g. for a looping shot). */
	virtual void StopSimulatingWeaponFire();

	virtual void DetermineWeaponState() override;

	virtual void SetWeaponState(EWeaponState::Type NewState) override;

	/** get the originating location for camera damage */
	FVector GetCameraDamageStartLocation(const FVector& AimDir) const;

	/** get the muzzle location of the weapon */
	FVector GetMuzzleLocation() const;

	/** get direction of weapon's muzzle */
	FVector GetMuzzleDirection() const;

	/** find hit */
	FHitResult WeaponTrace(const FVector& TraceFrom, const FVector& TraceTo) const;

protected:

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const;

	UFUNCTION()
	void OnRep_Reload();

	UFUNCTION()
	void OnRep_BurstCounter();


public:
	/**/
		/** get current ammo amount (total) */
	UFUNCTION(BlueprintCallable)
	int32 GetCurrentAmmo() const;

	/** get current ammo amount (clip) */
	UFUNCTION(BlueprintCallable)
	int32 GetCurrentAmmoInClip() const;

	/** get clip size */
	UFUNCTION(BlueprintCallable)
	int32 GetAmmoPerClip() const;

	/** get max ammo amount */
	UFUNCTION(BlueprintCallable)
	int32 GetMaxAmmo() const;

	/** check if weapon has infinite ammo (include owner's cheats) */
	UFUNCTION(BlueprintCallable)
	bool HasInfiniteAmmo() const;

	/** check if weapon has infinite clip (include owner's cheats) */
	UFUNCTION(BlueprintCallable)
	bool HasInfiniteClip() const;


public:
	//////////////////////////////////////////////////////////////////////////
	// Control

	/** check if weapon can fire */
	bool CanFire() const;

	/** check if weapon can be reloaded */
	bool CanReload() const;

protected:
	/** name of bone/socket for muzzle in weapon mesh */
	UPROPERTY(EditDefaultsOnly, Category = Effects)
	FName MuzzleAttachPoint;

	/** weapon data */
	UPROPERTY(EditDefaultsOnly, Category = Config)
	FWeaponData WeaponConfig;

	/** current total ammo */
	UPROPERTY(Transient, Replicated)
	int32 CurrentAmmo;

	/** current ammo - inside clip */
	UPROPERTY(Transient, Replicated)
	int32 CurrentAmmoInClip;

	/** burst counter, used for replicating fire events to remote clients */
	UPROPERTY(Transient, ReplicatedUsing = OnRep_BurstCounter)
	int32 BurstCounter;

	/** fire animations */
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	FWeaponAnim FireAnim;

	/** reload animations */
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	FWeaponAnim ReloadAnim;

	/** is fire animation playing? */
	bool bPlayingFireAnim;

	/** is fire animation looped? */
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	bool bLoopedFireAnim;

	/** is weapon fire active? */
	bool bWantsToFire;

	/** is reload animation playing? */
	UPROPERTY(Transient, ReplicatedUsing = OnRep_Reload)
	bool bPendingReload;

	/** weapon is refiring */
	bool bRefiring;

	/** time of last successful weapon fire */
	float LastFireTime;

	/** Handle for efficient management of StopReload timer */
	FTimerHandle TimerHandle_StopReload;

	/** Handle for efficient management of ReloadWeapon timer */
	FTimerHandle TimerHandle_ReloadWeapon;

	/** Handle for efficient management of HandleFiring timer */
	FTimerHandle TimerHandle_HandleFiring;

};
