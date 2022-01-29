// Fill out your copyright notice in the Description page of Project Settings.


#include "BeamParticleAbility.h"
#include "../Interfaces/AbilityUserInterface.h"
#include "../AbilityComponent.h"

#include "NiagaraFunctionLibrary.h"


void UBeamParticleAbility::StartBeam()
{
	UpdateBeamParameters();
	BeamComp->Activate();
}

void UBeamParticleAbility::SeverBeam()
{
	if (BeamComp != nullptr)
	{
		BeamComp->Deactivate();
	}
	AbilityComp->SetAbilityTarget(nullptr);
}

bool UBeamParticleAbility::IsTargetInRange() const
{
	bool retval = true;

	const IAbilityUserInterface * abilityuser = AbilityComp->GetAbilityUser();
	check(abilityuser);
	const FVector beamstart = abilityuser->GetAbilitySocketLocation(EffectSocketName);
	AActor * targetactor = AbilityComp->GetAbilityTarget();
	FHitResult lasthit;
	retval &= AbilityComp->GetHitInfoFor(targetactor, lasthit);

	if (retval == true)
	{
		const FVector targetpoint = lasthit.Location;
		const float currentbeamrange = FVector::Dist(targetpoint, beamstart);

		if (currentbeamrange > Range)
		{
			retval = false;
		}
	}



	return retval;
}

bool UBeamParticleAbility::IsTargetInLineOfSight() const
{
	bool retval = false;
	if (AbilityComp->GetAbilityTarget())
	{
		const IAbilityUserInterface * abilityuser = AbilityComp->GetAbilityUser();
		const AActor * targetactor = AbilityComp->GetAbilityTarget();
		checkf(abilityuser, TEXT("UBeamParticleAbility::IsTargetInLineOfSight Failed IAbuityUserInterface Cast!"));
		
		const FVector p1 = AbilityComp->GetCrosshairTransform(EffectSocketName).GetLocation();
		const FVector p2 = targetactor->GetActorLocation();

		const FVector v1 = abilityuser->GetAbilityAimVector().GetSafeNormal();
		const FVector v2 = (p2 - p1).GetSafeNormal();
		
		const float dot = FVector::DotProduct(v1, v2);
		float angle = FMath::Acos(dot);
		angle = FMath::RadiansToDegrees(angle);
		retval = FMath::IsNearlyEqual(0.0F, angle, BeamSeveranceAngle);
	}

	return retval;
}

UBeamParticleAbility::UBeamParticleAbility()
{
	BeamEmitter = CreateDefaultSubobject<UNiagaraSystem>(TEXT("BeamSystem"));
}

void UBeamParticleAbility::UpdateBeamParameters()
{
	if (BeamComp == nullptr) return;

	if (bLinkBeamToTarget && AbilityComp->GetAbilityTarget())
	{
		const AActor * Target = AbilityComp->GetAbilityTarget();
		const FVector endbeam = Target->GetActorLocation();
		const FName beamparametername = BeamEndParameterName;
		BeamComp->SetVectorParameter(beamparametername, endbeam);
	}
}

bool UBeamParticleAbility::ShouldSeverBeam() const
{
	bool retval = false;
	if (AbilityComp->GetAbilityTarget() != nullptr && (bLinkBeamToTarget == true))
	{
		if (!IsTargetInRange() || !IsTargetInLineOfSight() )
		{
			retval = true;
		}
	}
	else
	{
		retval = true;
	}

	return(retval);
}

void UBeamParticleAbility::Init(UAbilityComponent * InAbilityComp)
{
	Super::Init(InAbilityComp);
	IAbilityUserInterface * abilityuser = AbilityComp->GetAbilityUser();

	USceneComponent * attatchtocomp = abilityuser->GetParticleAttatchmentComponent();


	
	if (BeamEmitter != nullptr && attatchtocomp != nullptr)
	{
		EAttachLocation::Type attatchrule = EAttachLocation::SnapToTarget;

		if (IsValid(BeamEmitter) && attatchtocomp)
		{
			BeamComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
				BeamEmitter,
				attatchtocomp,
				EffectSocketName,
				EffectRelativeLocation,
				EffectRelativeRotation,
				attatchrule,
				false, //bAutoDestroy
				false  //bAutoActivate
			);
		}
	}
}

void UBeamParticleAbility::NotifyOnReady()
{
	/*If the beam is intended to hit a specific target then wait till the target is aquired*/
	if (BeamComp != nullptr && !bLinkBeamToTarget && !BeamComp->IsActive())
	{
		StartBeam();
	}
	Super::NotifyOnReady();
}

void UBeamParticleAbility::OnAbilityEnd()
{
	Super::OnAbilityEnd();
	SeverBeam();

}

void UBeamParticleAbility::ProcessTarget(AActor * Target)
{
	if (bLinkBeamToTarget == true && Target != nullptr && !BeamComp->IsActive())
	{
		StartBeam();
	}

}

void UBeamParticleAbility::UpdateChannel()
{
	Super::UpdateChannel();
	if ( BeamComp->IsActive() && ShouldSeverBeam())
	{
		SeverBeam();
	}
	else 
	{
		UpdateBeamParameters();
	}
}
