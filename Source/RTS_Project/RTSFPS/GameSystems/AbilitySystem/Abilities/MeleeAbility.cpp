// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeAbility.h"
#include "../AbilityComponent.h"

#include "Engine/DamageEvents.h"

void UMeleeAbility::OnTick(float InDeltaseconds)
{
	if (HasAuthority() && shouldtrace)
	{
		FHitResult outhit = FHitResult();
		const UWorld * world = GetWorld();
		const FVector starttrace = AbilityComp->GetAbilitySocketLocation(MeleeStartTraceSocketName);
		const FVector endtrace = AbilityComp->GetAbilitySocketLocation(MeleeEndTraceSocketName);

		FCollisionQueryParams queryparams = FCollisionQueryParams::DefaultQueryParam;
		//const FName TraceTag("DebugMeleeTag");
		//world->DebugDrawTraceTag = TraceTag;
		//queryparams.TraceTag = TraceTag;
		queryparams.bTraceComplex = false;
		queryparams.bReturnFaceIndex = true;
		queryparams.bReturnPhysicalMaterial = false;
		queryparams.bFindInitialOverlaps = false;

		/*Get the default ignore list, then add the actors we've already hit for the current sequence*/
		TArray ignoredactors = AbilityComp->GetIgnoredTraceActors(this);
		ignoredactors.Append(HitActors);
		queryparams.AddIgnoredActors(ignoredactors);
		world->LineTraceSingleByChannel(outhit, starttrace, endtrace, AbilityEffectChannel, queryparams);

		if (outhit.bBlockingHit)
		{
			ProcessHit(outhit);
		}
	}

}

void UMeleeAbility::OnAbilityEnd()
{
	HitActors.Reset();
	shouldtrace = true;
	CurrentActorHitCount = 0U;

	Super::OnAbilityEnd();
}

void UMeleeAbility::ProcessHit(const FHitResult& Inblockinghit)
{
	AActor* hitactor = Inblockinghit.GetActor();

	if (IsValid(hitactor) && hitactor->CanBeDamaged())
	{
		FDamageEvent damageevent = FDamageEvent();
		hitactor->TakeDamage(StaticCast<float>(Damage), damageevent, nullptr, AbilityComp->GetOwner());
		CurrentActorHitCount++;
		/*Stop tracing if we only hit the max number of targets*/
		if (CurrentActorHitCount >= MaxActorHitCount)
		{
			shouldtrace = false;
		}
		HitActors.Emplace(hitactor);
	}
}