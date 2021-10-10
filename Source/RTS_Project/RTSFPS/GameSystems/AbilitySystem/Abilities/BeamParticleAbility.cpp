// Fill out your copyright notice in the Description page of Project Settings.


#include "BeamParticleAbility.h"
#include "../Interfaces/AbilityUserInterface.h"
#include "../AbilityComponent.h"

#include "NiagaraFunctionLibrary.h"


UBeamParticleAbility::UBeamParticleAbility()
{
	BeamEmitter = CreateDefaultSubobject<UNiagaraSystem>(TEXT("BeamSystem"));
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
	if (BeamComp != nullptr)
	{
		BeamComp->Activate();
	}
}

void UBeamParticleAbility::OnAbilityEnd()
{
	Super::OnAbilityEnd();

	if (BeamComp != nullptr)
	{
		BeamComp->Deactivate();
	}
}
