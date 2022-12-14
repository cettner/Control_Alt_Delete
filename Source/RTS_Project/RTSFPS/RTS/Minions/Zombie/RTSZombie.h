// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../RTSMinion.h"
#include "RTSZombie.generated.h"

/**
 * 
 */

USTRUCT()
struct FAttackAnim
{
	GENERATED_USTRUCT_BODY()

		/**Attack Animaiton*/
		UPROPERTY(EditDefaultsOnly, Category = Animation)
		UAnimMontage* AttackAnim = nullptr;

		/*Time(s) During Attack Animation When Damage Checks Occur*/
		UPROPERTY(EditDefaultsOnly, Category = Animation)
		TArray<float> DamageEventTimes;

		/*Play Damage on Animation Completion*/
		UPROPERTY(EditDefaultsOnly, Category = Animation)
		bool PlayDamageOnEnd;

		bool operator==(const FAttackAnim& Other) const
		{
			return AttackAnim == Other.AttackAnim && DamageEventTimes == Other.DamageEventTimes && PlayDamageOnEnd == Other.PlayDamageOnEnd;
		}
};

USTRUCT()
struct FRepAnimInfo
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY()
	int AnimID = -1;
public:
	void EnsureReplication() { RepCounter++; }
private:
	UPROPERTY()
	int RepCounter = 0;
};


UCLASS()
class RTS_PROJECT_API ARTSZombie : public ARTSMinion
{
	GENERATED_BODY()

public:
	ARTSZombie(); 

public:
	virtual bool CanAttack(AActor * AttackMe) override;
	//virtual void StartAttack(AActor * AttackMe) override;
	virtual bool CanDoDamage(AActor * AttackMe) override;

	UFUNCTION()
	virtual void DoDamage(AActor * AttackMe, int ComboCount = 0, FAttackAnim Attack = FAttackAnim());

protected:
	virtual void OnAttackFinish();
	virtual FAttackAnim DecideAttack(AActor* AttackMe);
	virtual float GetDamage();
	UFUNCTION()
	virtual void PlayClientAnimEvent(int ComboCount, FAttackAnim Attack = FAttackAnim());
	virtual void PlayClientAnimEnd();

protected:
	UFUNCTION()
	virtual void OnRep_AnimID();
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	virtual void ReplicateAnim();

protected:
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	TArray<FAttackAnim> AttackVarients;

	UPROPERTY(EditDefaultsOnly, Category = Animation)
	float HitRange = 200.0F;

	bool bAttackAnimPlaying = false;
	
	UPROPERTY(EditDefaultsOnly, Category = Animation)
	bool bPlaysOnHitEffects = false;

	FAttackAnim CurrentAttack;

	UPROPERTY(Transient, ReplicatedUsing = OnRep_AnimID)
	struct FRepAnimInfo AnimInfo;

	FTimerHandle AttackEndHandler;
	FTimerHandle DamageEventHandler;
	FTimerDelegate ActorDelegate; 

private:
	bool IsFacingActor(AActor* OtherActor, float tolerance = 5.0F);

};
