// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "../../RTSMinion.h"
#include "../../Orders/RTSAttackOrder.h"
#include "RTSScion.generated.h"


struct FMeleeAttackSegmentData
{
	FString SegmentName = "";
	int32 CurrentHitCount = 0;
	int32 MaxHitCount = MAX_int32;
	bool HasReachedHitCount() const { return CurrentHitCount >= MaxHitCount; }
};

USTRUCT()
struct FMeleeAttackAnim
{
	GENERATED_USTRUCT_BODY()

		/**Attack Animaiton*/
		UPROPERTY(EditDefaultsOnly, Category = Animation)
		UAnimMontage* AttackAnim = nullptr;

		UPROPERTY(EditDefaultsOnly)
		FVector RootMotionShift = FVector(0.0f,0.0f,0.0f);

		UPROPERTY(EditDefaultsOnly)
		FRotator RootMotionRotation = FRotator(0.0f,0.0f,0.0f);

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
	void Set(const int32 InID, bool EnsureRep = true) 
	{
		AnimID = InID;

		if (EnsureRep == true)
		{
			RepCounter++;
		}
	}

	int32 Get() const
	{
		return AnimID;
	}

private:
	UPROPERTY()
	int32 AnimID = -1;

	UPROPERTY()
	uint8 RepCounter = 0;
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
		virtual bool ShouldPerformTrace() const override;
		virtual void OnAttackSegmentEnd(const UMeleeTraceNotifyState* InAttackNotify) override;
		virtual void OnAttackSegmentStart(const UMeleeTraceNotifyState* InAttackNotify) override;

	protected:
		virtual void DamageActor(AActor* InActor);
		virtual float GetDamage() const;

	protected:
		virtual bool CanDamageActor(const AActor * InTarget) const;
		virtual void OnAttackFinished();
		virtual bool CanAttackStopImmediately();
		virtual bool IsAttacking() const override;
	/********************************************************************************/

protected:
	virtual const TSubclassOf<URTSTargetedOrder> GetDefaultOrderClass(const FOrderContext& InHitContext) const override;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	/*Since Most of the calls for this class are called by the AIController for the server side only, this is a client only function,(listen server wont use this)*/
	UFUNCTION()
	virtual void OnRep_CurrentAttackIndex(FRepMeleeAnimInfo InPreviousIndex);

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

	UPROPERTY(EditDefaultsOnly)
	float Damage = 80.0f;

protected:
	UPROPERTY(EditDefaultsOnly, Category = Trace)
	TEnumAsByte<ECollisionChannel> WeaponTraceChannel = COLLISION_WEAPON;

	/*******************Runtime*********************/
protected:
	UPROPERTY(ReplicatedUsing = OnRep_CurrentAttackIndex)
	FRepMeleeAnimInfo CurrentAttackIndex = FRepMeleeAnimInfo();

	FTimerHandle AttackEndHandler = FTimerHandle();

	FMeleeAttackSegmentData AttackSegmentData = FMeleeAttackSegmentData();
};
