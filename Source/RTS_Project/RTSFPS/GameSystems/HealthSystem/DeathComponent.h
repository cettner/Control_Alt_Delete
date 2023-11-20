// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Animation/AnimMontage.h"
#include "Sound/SoundCue.h"
#include "GameFramework/MovementComponent.h"
#include "Engine/DamageEvents.h"


#include "DeathComponent.generated.h"


DECLARE_DELEGATE(FOnDeathStartDelegate);


UCLASS()
class RTS_PROJECT_API UDeathComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UDeathComponent();

	virtual void SetDeathanimMontage(UAnimMontage* InMontage);

	virtual bool IsAlive() const;

	/*Starts Death Process, can only be invoked on the server*/
	virtual bool Die();

protected:
	///////////////////////////////////////////////////////////////
	//Death

	virtual bool CanDie() const;

	/**Play Death Effects on Server + Client*/
	virtual void OnDeath();

	virtual void SetRagDollPhysics(USkeletalMeshComponent * Mesh = nullptr, UMovementComponent * Movement = nullptr);

	virtual void DestroyOwner(float timetilldestruction = 0.0F);

protected:
	virtual USkeletalMeshComponent * GetOwnerMesh();
	
	virtual UMovementComponent * GetOwnerMovement();

	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

protected:
	UFUNCTION()
	virtual void OnRep_bIsDying();

public:
	FOnDeathStartDelegate OnDeathStart;

protected:

	//////////////////////////////////////////////////
	//Death

	UPROPERTY(EditDefaultsOnly, Category = "Death")
	bool bShouldRagdoll = false;

	UPROPERTY(EditDefaultsOnly, Category = "Death", meta = (EditCondition = "bShouldRagDoll"))
	bool bDestroyAfterRagdoll = true;

	UPROPERTY(EditDefaultsOnly, Category = "Death")
	float TimeRagdoll = 5.0F;

	/** Animation Played On Pawn Mesh Before Destruction/RagDoll*/
	UPROPERTY(EditDefaultsOnly, Category = "Death")
	UAnimMontage* DeathAnimMontage;

	/** Sound played on death, local player only */
	UPROPERTY(EditDefaultsOnly, Category = "Death")
	USoundCue* DeathSound;

	/*If death effects are currently playing*/
	UPROPERTY(ReplicatedUsing = OnRep_bIsDying)
	bool bIsDying = false;

private:
	/*Prepares Owner Comp for Ragdoll Physics*/
	void HandleRagDoll(USkeletalMeshComponent * Mesh, const float DeathAnimDuration = 0.0F);

	/*Wrapper Delegate for SetRagdollPhysics*/
	void RagDollTimerHandler();
};
