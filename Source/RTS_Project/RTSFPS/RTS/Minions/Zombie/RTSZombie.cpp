// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSZombie.h"
#include "RTS_Project/RTSFPS/RTS/Minions/AI/RTSAIController.h"
#include "Net/UnrealNetwork.h"

ARTSZombie::ARTSZombie() : Super()
{
	bReplicates = true;
}

/*
bool ARTSZombie::CanAttack(AActor* AttackMe)
{
	bool attack = false;
	if (AttackMe && CanDoDamage(AttackMe))
	{
		bool distcheck = false;
		float distance = GetDistanceTo(AttackMe);
		FVector pointofimpact = FVector(0, 0, 0);
		float collisiondistance = AttackMe->ActorGetDistanceToCollision(GetActorLocation(), ECollisionChannel::ECC_Pawn, pointofimpact);
		distcheck = collisiondistance <= HitRange;

		bool dircheck = IsFacingActor(AttackMe, 15.0F);
		attack = (distcheck && dircheck);
	}

	return(attack);
}

void ARTSZombie::StartAttack(AActor* AttackMe)
{
	if (AttackVarients.Num() && !bAttackAnimPlaying && GetWorld())
	{
		FAttackAnim Attack = DecideAttack(AttackMe);

		if (Attack.AttackAnim)
		{
			bAttackAnimPlaying = true;
			CurrentAttack = Attack;

			float EndAnimTime = PlayAnimMontage(Attack.AttackAnim);
			ReplicateAnim();

			if (Attack.DamageEventTimes.Num())
			{
				float FirstEvent = Attack.DamageEventTimes[0];
				ActorDelegate.BindUFunction(this, FName("DoDamage"), AttackMe, (int)0, Attack);
				GetWorldTimerManager().SetTimer(DamageEventHandler, ActorDelegate, FirstEvent, false);
			}

			GetWorldTimerManager().SetTimer(AttackEndHandler, this, &ARTSZombie::OnAttackFinish, EndAnimTime, false);
		}
	}

	if (!AttackEndHandler.IsValid())
	{
		ARTSAIController* AIC = Cast<ARTSAIController>(GetController());
		if (AIC)
		{
			AIC->SendAIMessage(ARTSAIController::AIMessage_Finished, FAIMessage::EStatus::Failure);
		}
	}
}

bool ARTSZombie::CanDoDamage(AActor* AttackMe)
{	
	return(IsEnemy(AttackMe) && (AttackVarients.Num() > 0));
}
*/

/*
void ARTSZombie::ReleaseAssets()
{
	Super::ReleaseAssets();
	
	if (GetWorldTimerManager().IsTimerActive(AttackEndHandler) || GetWorldTimerManager().IsTimerActive(DamageEventHandler))
	{
		ARTSAIController* AIC = Cast<ARTSAIController>(GetController());
		if (AIC)
		{
			AIC->SendAIMessage(ARTSAIController::AIMessage_Finished, FAIMessage::EStatus::Failure);
		}
	}
	GetWorldTimerManager().ClearTimer(AttackEndHandler);
	GetWorldTimerManager().ClearTimer(DamageEventHandler);
	bAttackAnimPlaying = false;
}
*/
void ARTSZombie::DoDamage(AActor* AttackMe, int ComboCount, FAttackAnim Attack)
{
	if (!this->IsValidLowLevel() || !this->IsAlive()) return;
	/*
	if (CanAttack(AttackMe))
	{	
		FDamageEvent DE;
		AttackMe->TakeDamage(GetDamage(), DE, GetController(), this);
	}
	*/
	if (ComboCount < Attack.DamageEventTimes.Num() && IsValid(AttackMe))
	{
		ActorDelegate.BindUFunction(this, FName("DoDamage"), AttackMe, ComboCount++, Attack);
		float NextEvent = Attack.DamageEventTimes[ComboCount++];
		GetWorldTimerManager().SetTimer(DamageEventHandler, ActorDelegate, NextEvent, false);
	}
}

FAttackAnim ARTSZombie::DecideAttack(AActor* AttackMe)
{
	FAttackAnim Attack;
	if (AttackVarients.Num())
	{
		int attack_index = FMath::Rand() % AttackVarients.Num();
		Attack = AttackVarients[attack_index];
	}
	return(Attack);
}

float ARTSZombie::GetDamage()
{
	return(10.0F);
}

void ARTSZombie::PlayClientAnimEvent(int ComboCount, FAttackAnim Attack)
{
	if (ComboCount < Attack.DamageEventTimes.Num())
	{
		float NextEvent = Attack.DamageEventTimes[ComboCount];
		ActorDelegate.BindUFunction(this, FName("PlayClientAnimEvent"), ComboCount++, Attack);
		GetWorldTimerManager().SetTimer(DamageEventHandler, ActorDelegate, NextEvent, false);
	}
}

void ARTSZombie::PlayClientAnimEnd()
{
	CurrentAttack = FAttackAnim();
}

void ARTSZombie::OnRep_AnimID()
{
	if (AnimInfo.AnimID > -1 && AnimInfo.AnimID < AttackVarients.Num())
	{
		CurrentAttack = AttackVarients[AnimInfo.AnimID];
		if (CurrentAttack.AttackAnim)
		{
			float EndAnimTime;
			EndAnimTime = PlayAnimMontage(CurrentAttack.AttackAnim);
			GetWorldTimerManager().SetTimer(AttackEndHandler, this, &ARTSZombie::OnAttackFinish, EndAnimTime, false);
		}

		if (CurrentAttack.DamageEventTimes.Num() && bPlaysOnHitEffects)
		{
			float FirstEvent = CurrentAttack.DamageEventTimes[0];
			ActorDelegate.BindUFunction(this, FName("PlayClientAnimEvent"), (int)0, AttackVarients[AnimInfo.AnimID]);
			GetWorldTimerManager().SetTimer(DamageEventHandler, ActorDelegate, FirstEvent, false);
		}
	}
}

void ARTSZombie::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ARTSZombie, AnimInfo)
}

void ARTSZombie::ReplicateAnim()
{
	    AnimInfo.AnimID = AttackVarients.Find(CurrentAttack);
		AnimInfo.EnsureReplication();
}

void ARTSZombie::OnAttackFinish()
{
	bAttackAnimPlaying = false;
	if (CurrentAttack.PlayDamageOnEnd)
	{
		//DoDamage(GetTarget());
	}
	CurrentAttack = FAttackAnim();

	ARTSAIController* AIC = Cast<ARTSAIController>(GetController());
	if (AIC)
	{
		AIC->SendAIMessage(ARTSAIController::AIMessageOrderRequest, FAIMessage::EStatus::Success, EAIMessageType::Task);
	}
}

bool ARTSZombie::IsFacingActor(AActor* OtherActor, float tolerance)
{
	FVector FV = GetActorForwardVector(); 
	FVector Diff =  OtherActor->GetActorLocation() - GetActorLocation();

	Diff = Diff.GetSafeNormal();

	float dot = Diff.DotProduct(FV, Diff);
	float angle = FMath::Acos(dot);

	return (FMath::IsNearlyEqual(0.0F, angle, tolerance));
}
