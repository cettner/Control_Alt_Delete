// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSScion.h"
#include "RTS_Project/RTSFPS/RTS/Minions/AI/RTSAIController.h"

#include "Engine/DamageEvents.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/CharacterMovementComponent.h"

ARTSScion::ARTSScion()
{
	GetCharacterMovement()->MaxWalkSpeed = 200.0f;
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeleeScanWeapon"));
	WeaponMesh->SetReceivesDecals(false);
	WeaponMesh->SetupAttachment(GetMesh(), "WeaponSocket");

	AIConfig.SightAffiliation.bDetectNeutrals = false;
	AIConfig.SightAffiliation.bDetectEnemies = true;
	AIConfig.SightAffiliation.bDetectFriendlies = false;

	AISenseClasses.Emplace(UAISense_Sight::StaticClass());

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
		CurrentAttackIndex.Set(usedattackindex, true);
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
		UAnimMontage* currentattackanim = MeleeAttacks[CurrentAttackIndex.Get()].AttackAnim;
		StopAnimMontage(currentattackanim);
		GetWorldTimerManager().ClearTimer(AttackEndHandler);
		CurrentAttackIndex.Set(-1,true);
		AttackSegmentData = FMeleeAttackSegmentData();
	}

	return retval;
}

FHitResult ARTSScion::PerformTrace()
{
	FHitResult outhit = FHitResult();
	UWorld* world = GetWorld();
	if (IsValid(world)) //Editor Failure Check
	{
			const FVector starttrace = WeaponMesh->GetSocketLocation(WeaponTraceStart);
			const FVector endtrace = WeaponMesh->GetSocketLocation(WeaponTraceEnd);
			const FCollisionShape traceshape = FCollisionShape::MakeSphere(30.0f);
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

	if (outhit.bBlockingHit && CanDamageActor(outhit.GetActor()))
	{
		AttackSegmentData.CurrentHitCount++;
		DamageActor(outhit.GetActor());
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

bool ARTSScion::ShouldPerformTrace() const
{
	bool retval = HasAuthority() && !AttackSegmentData.HasReachedHitCount();
	return retval;
}

void ARTSScion::OnAttackSegmentStart(const UMeleeTraceNotifyState* InAttackNotify)
{
	AttackSegmentData.CurrentHitCount = 0;
	AttackSegmentData.MaxHitCount = InAttackNotify->GetMaxSegmentHitCount();
	AttackSegmentData.SegmentName = InAttackNotify->GetNotifyName();
}

void ARTSScion::DamageActor(AActor* InActor)
{
	const float damage = GetDamage();
	InActor->TakeDamage(damage,FDamageEvent(),GetController(),this);
}

float ARTSScion::GetDamage() const
{
	return Damage;
}

void ARTSScion::OnAttackSegmentEnd(const UMeleeTraceNotifyState* InAttackNotify)
{
	AttackSegmentData = FMeleeAttackSegmentData();
}

bool ARTSScion::CanDamageActor(const AActor* InTarget) const
{
	bool retval = false;
	retval = IsEnemy(InTarget);
	
	return retval;
}

void ARTSScion::OnAttackFinished()
{
	CurrentAttackIndex.Set(-1,false);
	AttackSegmentData = FMeleeAttackSegmentData();
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
	bool retval = CurrentAttackIndex.Get() == CANT_ATTACK_INDEX;

	if (!retval)
	{
		FMeleeAttackAnim currentattack = MeleeAttacks[CurrentAttackIndex.Get()];
		retval = currentattack.bCanAnimAbortImmediately;
	}

	return retval;
}

bool ARTSScion::IsAttacking() const
{
	return CurrentAttackIndex.Get() != CANT_ATTACK_INDEX;
}

const TSubclassOf<URTSTargetedOrder> ARTSScion::GetDefaultOrderClass(const FOrderContext& InHitContext) const
{
	TSubclassOf<URTSTargetedOrder> retval = nullptr;
	if (IsEnemy(InHitContext.GetRTSContext()))
	{
		retval = AttackOrderClass;
	}
	else
	{
		retval = Super::GetDefaultOrderClass(InHitContext);
	}

	return retval;
}

void ARTSScion::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ARTSScion, CurrentAttackIndex);
}

void ARTSScion::OnRep_CurrentAttackIndex(FRepMeleeAnimInfo PreviousIndex)
{
	if (CurrentAttackIndex.Get() != CANT_ATTACK_INDEX)
	{
		UAnimMontage* currentattackanim = MeleeAttacks[CurrentAttackIndex.Get()].AttackAnim;
		const float endanimtime = PlayAnimMontage(currentattackanim);
	}
	else if (PreviousIndex.Get() != CANT_ATTACK_INDEX)
	{
		UAnimMontage* currentattackanim = MeleeAttacks[PreviousIndex.Get()].AttackAnim;
		StopAnimMontage(currentattackanim);
	}
}
