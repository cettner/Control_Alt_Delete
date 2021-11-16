// Fill out your copyright notice in the Description page of Project Settings.


#include "TraceTargetAbility.h"
#include "../AbilityComponent.h"


void UTraceTargetAbility::OnEffect()
{
	const FCollisionShape traceshape = FCollisionShape::MakeCapsule(1.0, 1.0);
	FCollisionQueryParams queryparams = FCollisionQueryParams::DefaultQueryParam;
	IAbilityUserInterface * abilityuser = AbilityComp->GetAbilityUser();
	check(abilityuser);

	const FVector spawnlocation = abilityuser->GetAbilitySocketLocation(EffectSocketName);
	const FVector aimdirection = abilityuser->GetAbilityAimVector();
	const FVector endtrace = spawnlocation + (aimdirection * Range);

	UWorld * world = GetWorld();
	FHitResult outhit;

	const FName TraceTag("DebugShooterTag");
	world->DebugDrawTraceTag = TraceTag;
	queryparams.TraceTag = TraceTag;


	queryparams.AddIgnoredActors(abilityuser->GetIgnoredTraceActors(TWeakObjectPtr<UAbility>(this)));

	world->SweepSingleByChannel(outhit, spawnlocation, endtrace, aimdirection.ToOrientationQuat(), GetAbilityCollisionChannel(), traceshape, queryparams);

	if (bOnlyProcessTrueHits == true)
	{
		if (outhit.bBlockingHit == true)
		{
			ProcessTraceHit(outhit,spawnlocation,endtrace);
		}
	}
	else 
	{
		ProcessTraceHit(outhit, spawnlocation, endtrace);
	}
	
}

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
	Super::OnAbilityEnd();
}

void UTraceTargetAbility::ProcessTraceHit(FHitResult HitResult, FVector StartTrace, FVector EndTrace)
{
	if(CanHit(HitResult))
	{
		AbilityComp->SetAbilityTarget(HitResult.GetActor());
	}
}

bool UTraceTargetAbility::CanHit(AActor * HitActor)
{
	return true;
}

void UTraceTargetAbility::UpdateChannel()
{
	OnEffect();
}