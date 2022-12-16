// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSScion.h"
#include "RTS_Project/RTSFPS/RTS/Minions/AI/RTSAIController.h"

#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"

ARTSScion::ARTSScion()
{
	GetCharacterMovement()->MaxWalkSpeed = 200.0f;
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeleeScanWeapon"));
	WeaponMesh->SetReceivesDecals(false);
	WeaponMesh->SetupAttachment(GetMesh(), "WeaponSocket");

}

bool ARTSScion::StartAttack(const int32 InAttackID)
{
	UAnimMontage* montagetoplay = nullptr;
	bool retval = false;
	int32 usedattackindex = -1;

	if (InAttackID > CANT_ATTACK_INDEX)
	{
		usedattackindex = InAttackID;
		montagetoplay = MeleeAttacks[usedattackindex].AttackAnim;
	}
	else if (MeleeAttacks.Num() > 0)
	{
		usedattackindex = 0;
		montagetoplay = MeleeAttacks[usedattackindex].AttackAnim;
	}

	if (montagetoplay != nullptr)
	{
		CurrentAttackIndex = usedattackindex;
		const float endanimtime = PlayAnimMontage(montagetoplay);
		GetWorldTimerManager().SetTimer(AttackEndHandler, this, &ARTSScion::OnAttackFinished, endanimtime, false);
		retval = endanimtime > 0.0f;
	}

	return retval;
}

bool ARTSScion::StopAttack(const bool InForceStop)
{
	bool retval = false;
	if (CanAttackStopImmediately())
	{
		retval = true;
		UAnimMontage* currentattackanim = MeleeAttacks[CurrentAttackIndex].AttackAnim;
		StopAnimMontage(currentattackanim);
		GetWorldTimerManager().ClearTimer(AttackEndHandler);
		CurrentAttackIndex = -1;
	}

	return retval;
}

FHitResult ARTSScion::PerformTrace()
{
	UWorld* world = GetWorld();
	FHitResult outhit = FHitResult();
	if (IsValid(world)) //Editor Failure Check
	{
		const FVector starttrace = WeaponMesh->GetSocketLocation(WeaponTraceStart);
		const FVector endtrace = WeaponMesh->GetSocketLocation(WeaponTraceEnd);
		const FCollisionShape traceshape = FCollisionShape::MakeSphere(15.0f);
		FCollisionQueryParams queryparams = FCollisionQueryParams::DefaultQueryParam;
		FName TraceTag("DebugMeleeTag");
		world->DebugDrawTraceTag = TraceTag;
		queryparams.TraceTag = TraceTag;
		queryparams.bTraceComplex = false;
		queryparams.bReturnFaceIndex = true;
		queryparams.bReturnPhysicalMaterial = false;
		queryparams.bFindInitialOverlaps = false;
		const FQuat tracerotation = FRotator(0.0f, 0.0f, 0.0f).Quaternion();

		world->SweepSingleByChannel(outhit, starttrace, endtrace, tracerotation, WeaponTraceChannel, traceshape, queryparams);
	}
	return outhit;
}

int32 ARTSScion::GetAttackIndexForTarget(const AActor* InToAttack) const
{
	int32 retval = CANT_ATTACK_INDEX;
	if (!IsAttacking() && MeleeAttacks.Num())
	{
		float distance = GetDistanceTo(InToAttack);
		if (distance <= AttackRange)
		{
			retval = 0;
		}
	}
	return retval;
}

void ARTSScion::OnAttackFinished()
{
	CurrentAttackIndex = -1;
	ARTSAIController* AIC = Cast<ARTSAIController>(GetController());
	/*Editor Protection for Anim-Notifies*/
	if (AIC != nullptr)
	{
		if (AIC->IsAbortingTask())
		{
			AIC->SendAIMessage(ARTSAIController::AIMessageAbortRequest, FAIMessage::EStatus::Success, EAIMessageType::Abort);
		}
		else
		{
			AIC->SendAIMessage(ARTSAIController::AIMessageOrderRequest, FAIMessage::EStatus::Success, EAIMessageType::Task);
		}
	}
}

bool ARTSScion::CanAttackStopImmediately()
{
	bool retval = CurrentAttackIndex == CANT_ATTACK_INDEX;

	if (!retval)
	{
		FMeleeAttackAnim currentattack = MeleeAttacks[CurrentAttackIndex];
		retval = currentattack.bCanAnimAbortImmediately;
	}

	return retval;
}

bool ARTSScion::IsAttacking() const
{
	return CurrentAttackIndex != CANT_ATTACK_INDEX;
}

const TSubclassOf<URTSTargetedOrder> ARTSScion::GetDefaultOrderClass(const FHitResult& InHitContext) const
{
	TSubclassOf<URTSTargetedOrder> retval = nullptr;
	if (IsEnemy(InHitContext.GetActor()))
	{
		retval = AttackOrderClass;
	}
	else
	{
		retval = Super::GetDefaultOrderClass(InHitContext);
	}

	return retval;
}
