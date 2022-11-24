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
		UAnimMontage* AttackAnim;

		UPROPERTY(EditDefaultsOnly)
		FVector RootMotionShift = FVector();

		UPROPERTY(EditDefaultsOnly)
		FRotator RootMotionRotation = FRotator();

		int32 AnimUID = -1;

		bool operator==(const FMeleeAttackAnim& Other) const
		{
			return AnimUID == Other.AnimUID;
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
	protected:
		virtual void PostInitializeComponents() override;

	protected:
		virtual bool StartAttack(const int32 InAttackID = -1) override;
		virtual FHitResult PerformTrace() override;
	
	protected:
		virtual int32 GetAttackIndexForTarget(const AActor* InToAttack) const;
		virtual void OnAttackFinished();
		bool IsAttacking() const;

protected:
	virtual const TSubclassOf<URTSTargetedOrder> GetDefaultOrderClass(const FHitResult& InHitContext) const override;


protected:
	UPROPERTY(EditDefaultsOnly)
	USkeletalMeshComponent* WeaponMesh = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FName WeaponTraceStart = "TraceStart";

	UPROPERTY(EditDefaultsOnly)
	FName WeaponTraceEnd = "TraceEnd";

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<URTSAttackOrder> AttackOrderClass = URTSAttackOrder::StaticClass();

	UPROPERTY(EditDefaultsOnly)
	TArray<FMeleeAttackAnim> MeleeAttacks = TArray<FMeleeAttackAnim>();

	UPROPERTY(EditDefaultsOnly)
	float AttackRange = 200.0f;

protected:
	UPROPERTY(EditDefaultsOnly, Category = Trace)
	TEnumAsByte<ECollisionChannel> WeaponTraceChannel;

protected:
	UAnimMontage* CurrentAttackAnim = nullptr;
	FTimerHandle AttackEndHandler;
};
