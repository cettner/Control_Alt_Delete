// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Animation/AnimMontage.h"
#include "Sound/SoundCue.h"
#include "GameFramework/MovementComponent.h"
#include "UObject/ObjectMacros.h"
#include "HealthComponent.generated.h"


DECLARE_DELEGATE(FOnDeathStartDelegate);

/** replicated information on a hit we've taken */
USTRUCT()
struct FTakeHitInfo
{
	GENERATED_USTRUCT_BODY()

	/** The amount of damage actually applied */
	UPROPERTY()
	float ActualDamage;

	/** The damage type we were hit with. */
	UPROPERTY()
	UClass* DamageTypeClass;

	/** Who hit us */
	UPROPERTY()
	TWeakObjectPtr<APawn> PawnInstigator;

	/** Who actually caused the damage */
	UPROPERTY()
	TWeakObjectPtr<AActor> DamageCauser;

	/** Specifies which DamageEvent below describes the damage received. */
	UPROPERTY()
	int32 DamageEventClassID;

	/** Whether this was a kill */
	UPROPERTY()
	bool bKilled;

private:
	/** A rolling counter used to ensure the struct is dirty and will replicate. */
	UPROPERTY()
	uint8 EnsureReplicationByte;

public:
	void EnsureReplication() { EnsureReplicationByte++; }
	/** defaults */
	FTakeHitInfo()
	{
		ActualDamage = 0;
		DamageTypeClass = NULL;
		PawnInstigator = NULL;
		DamageCauser = NULL;
		DamageEventClassID = 0;
		bKilled = false;
		EnsureReplicationByte = 0;
	}
};




UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RTS_PROJECT_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

	virtual void SetMaxHealth(float healthval);

	virtual void SetDeathanimMontage(UAnimMontage* InMontage);

	virtual float GetMaxHealth() const;

	virtual float GetCurrentHealth() const;

	virtual void SetCurrentHealth(float InHealth);

	virtual float GetHealthPercentage() const;

	virtual bool IsAlive() const;

	virtual float HandleDamageEvent(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

protected:
	///////////////////////////////////////////////////////////////
	//Death

	virtual bool CanDie(float KillingDamage, FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser) const;

	/*Starts Death Process, can only be invoked on the server*/
	virtual bool Die(float KillingDamage, struct FDamageEvent const& DamageEvent, AController* Killer, AActor* DamageCauser);

	/**Play Death Effects on Server + Client*/
	virtual void OnDeath(float KillingDamage, struct FDamageEvent const& DamageEvent, class APawn* InstigatingPawn, class AActor* DamageCauser);

	virtual void SetRagDollPhysics(USkeletalMeshComponent * Mesh = nullptr, UMovementComponent * Movement = nullptr);

	virtual void DestroyOwner(float timetilldestruction = 0.0F);

public:
	//////////////////////////////////////////////////
    //Damage
	/** sets up the replication for taking a hit */
	void ReplicateHit(float Damage, struct FDamageEvent const& DamageEvent, class APawn* InstigatingPawn, class AActor* DamageCauser, bool bKilled);

	/* Play on hit effects on the client*/
	void PlayLocalHit(float Damage, FDamageEvent const& DamageEvent, APawn* InstigatingPawn, AActor* DamageCauser);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

protected:
	virtual float ModifyDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

	virtual USkeletalMeshComponent * GetOwnerMesh();
	
	virtual UMovementComponent * GetOwnerMovement();
protected:
	UFUNCTION()
	virtual void  OnRep_LastTakeHitInfo();

	virtual void PreReplication(IRepChangedPropertyTracker & ChangedPropertyTracker) override;

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

public:
	FOnDeathStartDelegate OnDeathStart;

protected:
	/////////////////////////////////////////////////
	//Components
	UPROPERTY(Transient,BlueprintReadWrite)
	AActor * CompOwner;

	/////////////////////////////////////////////////
	//Life
	UPROPERTY(BlueprintReadWrite, Replicated, Category = "Health")
	float CurrentHealth;

	UPROPERTY(EditDefaultsOnly, Replicated, Category="Health")
	float MaxHealth;

	UPROPERTY(EditDefaultsOnly, Category = "Health")
	TMap<TSubclassOf<UDamageType>, float> Mitigation;

	//////////////////////////////////////////////////
	//Damage

	/** Replicate where this pawn was last hit and damaged */
	UPROPERTY(Transient, ReplicatedUsing = OnRep_LastTakeHitInfo)
	struct FTakeHitInfo LastTakeHitInfo;

	/** Time at which point the last take hit info for the actor times out and won't be replicated;*/
	float LastTakeHitTimeTimeout;


	//////////////////////////////////////////////////
	//Death

	UPROPERTY(EditDefaultsOnly, Category = "Death")
	bool ShouldRagdoll = true;

	UPROPERTY(EditDefaultsOnly, Category = "Death")
	bool DestroyAfterRagdoll = true;

	UPROPERTY(EditDefaultsOnly, Category = "Death")
	float TimeRagdoll = 5.0F;

	/** Animation Played On Pawn Mesh Before Destruction/RagDoll*/
	UPROPERTY(EditDefaultsOnly, Category = "Death")
	UAnimMontage* DeathAnimMontage;

	/** Sound played on death, local player only */
	UPROPERTY(EditDefaultsOnly, Category = "Death")
	USoundCue* DeathSound;

	/*If death effects are currently playing*/
	bool bIsDying = false;

private:
	/*Prepares Owner Comp for Ragdoll Physics*/
	void HandleRagDoll(USkeletalMeshComponent * Mesh, const float DeathAnimDuration = 0.0F);

	/*Wrapper Delegate for SetRagdollPhysics*/
	void RagDollTimerHandler();
};
