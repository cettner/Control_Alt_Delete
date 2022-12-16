// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "../../RTSMinion.h"
#include "../../Orders/RTSAttackOrder.h"
#include "RTSScion.generated.h"

/**
 * 
 */

USTRUCT()
struct FMeleeAttackAnim
{
	GENERATED_USTRUCT_BODY()

		/**Attack Animaiton*/
		UPROPERTY(EditDefaultsOnly, Category = Animation)
		UAnimMontage* AttackAnim = nullptr;

		UPROPERTY(EditDefaultsOnly)
		FVector RootMotionShift = FVector();

		UPROPERTY(EditDefaultsOnly)
		FRotator RootMotionRotation = FRotator();

		UPROPERTY(EditDefaultsOnly)
		bool bCanAnimAbortImmediately = true;

		bool operator==(const FMeleeAttackAnim& Other) const
		{
			return AttackAnim == Other.AttackAnim;
		}

		bool operator==(const UAnimMontage*& OtherAnim) const
		{
			return AttackAnim == OtherAnim;
		}
};

USTRUCT()
struct FRepMeleeAnimInfo
{
	GENERATED_USTRUCT_BODY()
public:
	UPROPERTY()
	int32 AnimID = -1;
public:
	void EnsureReplication() { RepCounter++; }
private:
	UPROPERTY()
	int RepCounter = 0;
};


UCLASS()
class RTS_PROJECT_API ARTSScion : public ARTSMinion 
{
	GENERATED_BODY()
	
	ARTSScion();

	/**********************************************************************************/
	protected:
		virtual bool StartAttack(const int32 InAttackID = -1) override;
		virtual bool StopAttack(const bool InForceStop = false) override;
		virtual FHitResult PerformTrace() override;
		virtual int32 GetAttackIndexForTarget(const AActor* InToAttack) const override;

	protected:
		virtual void OnAttackFinished();
		virtual bool CanAttackStopImmediately();
		virtual bool IsAttacking() const override;
	/********************************************************************************/

protected:
	virtual const TSubclassOf<URTSTargetedOrder> GetDefaultOrderClass(const FHitResult& InHitContext) const override;


protected:
	UPROPERTY(EditDefaultsOnly)
	USkeletalMeshComponent* WeaponMesh = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FName WeaponTraceStart = "TraceStart";

	UPROPERTY(EditDefaultsOnly)
	FName WeaponTraceEnd = "TraceEnd";

	UPROPERTY(EditDefaultsOnly, Category = Orders)
	TSubclassOf<URTSAttackOrder> AttackOrderClass = URTSAttackOrder::StaticClass();

	UPROPERTY(EditDefaultsOnly)
	TArray<FMeleeAttackAnim> MeleeAttacks = TArray<FMeleeAttackAnim>();

	UPROPERTY(EditDefaultsOnly)
	float AttackRange = 200.0f;

protected:
	UPROPERTY(EditDefaultsOnly, Category = Trace)
	TEnumAsByte<ECollisionChannel> WeaponTraceChannel = COLLISION_WEAPON;

protected:
	int32 CurrentAttackIndex = -1;

	FTimerHandle AttackEndHandler = FTimerHandle();
};
