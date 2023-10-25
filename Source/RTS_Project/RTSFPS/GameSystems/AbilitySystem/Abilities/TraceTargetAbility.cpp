// Fill out your copyright notice in the Description page of Project Settings.


#include "TraceTargetAbility.h"
#include "../AbilityComponent.h"
#include "Net/UnrealNetwork.h"


void UTraceTargetAbility::NotifyOnReady()
{
	float indelay = -1.0F;
	if (bUpdatesOnReady == true)
	{
		indelay = 0.0F;
	}
	GetWorld()->GetTimerManager().SetTimer(ChannelPulseHandler,this,&UTraceTargetAbility::UpdateChannel, PulseRate, true, indelay);
}

void UTraceTargetAbility::OnAbilityEnd()
{
	GetWorld()->GetTimerManager().ClearTimer(ChannelPulseHandler);
	SetAbilityTarget(nullptr);

	Super::OnAbilityEnd();
}

void UTraceTargetAbility::ProcessTraceHit(FHitResult HitResult, FVector StartTrace, FVector EndTrace)
{
	if(CanHit(HitResult.GetActor()))
	{
		SetAbilityTarget(HitResult.GetActor(), HitResult);
		ProcessTarget(HitResult.GetActor());
	}

}

bool UTraceTargetAbility::GetHitInfoFor(AActor* HitTarget, FHitResult& Hit) const
{
	bool retval = false;
	if (LastAbilityHit.GetActor() == HitTarget)
	{
		Hit = LastAbilityHit;
		retval = true;
	}
	return retval;
}

bool UTraceTargetAbility::CanHit(AActor * HitActor) const
{
	return true;
}

void UTraceTargetAbility::OnRep_AbilityTarget()
{
	ProcessTarget(AbilityTarget);
}

void UTraceTargetAbility::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UTraceTargetAbility, AbilityTarget);
}

void UTraceTargetAbility::UpdateChannel()
{
	const FCollisionShape traceshape = FCollisionShape::MakeCapsule(1.0, 1.0);
	FCollisionQueryParams queryparams = FCollisionQueryParams::DefaultQueryParam;
	IAbilityUserInterface * abilityuser = AbilityComp->GetAbilityUser();
	check(abilityuser);

	const FVector spawnlocation = abilityuser->GetAbilitySocketLocation(EffectSocketName);
	const FVector aimdirection = abilityuser->GetAbilityAimVector();
	const FVector endtrace = spawnlocation + (aimdirection * Range);

	const UWorld * world = GetWorld();
	FHitResult outhit;

	queryparams.AddIgnoredActors(abilityuser->GetIgnoredTraceActors(TWeakObjectPtr<UAbility>(this)));

	world->SweepSingleByChannel(outhit, spawnlocation, endtrace, aimdirection.ToOrientationQuat(), GetAbilityCollisionChannel(), traceshape, queryparams);

	if (bOnlyProcessTrueHits == true)
	{
		if (outhit.bBlockingHit == true)
		{
			ProcessTraceHit(outhit, spawnlocation, endtrace);
		}
	}
	else
	{
		ProcessTraceHit(outhit, spawnlocation, endtrace);
	}
}

void UTraceTargetAbility::SetAbilityTarget(AActor* NewTarget, FHitResult AssociatedHit)
{
	if (HasAuthority())
	{
		AbilityTarget = NewTarget;
	}

	LastAbilityHit = AssociatedHit;
}
