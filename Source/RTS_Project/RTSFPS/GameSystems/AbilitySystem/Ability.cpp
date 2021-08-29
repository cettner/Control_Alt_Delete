// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability.h"
#include "AbilityComponent.h"
#include "Animation/AbilityAnimNotify.h"

void UAbility::Init(UAbilityComponent * InComp)
{
	AbilityComp = InComp;
}

void UAbility::OnAbilityStart()
{
	if (StartAbilityMontages.Num())
	{
		float playtime = AbilityComp->PlayAbilityMontage(StartAbilityMontages[0]);
		if (playtime > 0.0f)
		{
			AbilityComp->OnCastStart();
		}
	}
}

void UAbility::OnEffect()
{

}

void UAbility::OnAbilityReleased()
{
	bool bcanplaymontage = !bRequiresReadyCast | AbilityComp->IsCastReady();

	if (ReleaseAbilityMontages.Num() && bcanplaymontage)
	{
		AbilityComp->SetIsCastSuccessful(true);
		float playtime = AbilityComp->PlayAbilityMontage(ReleaseAbilityMontages[0]);
	}
	else
	{
		AbilityComp->SetIsCastSuccessful(false);
		AbilityComp->StopCurrentAnimation();
		AbilityComp->OnCastEnd();
	}
}

void UAbility::OnAbilityEnd()
{
	AbilityComp->OnCastEnd();
}

void UAbility::NotifyOnReady()
{
	AbilityComp->PlayAbilityMontage(LoopMontage);
}

void UAbility::NotifyOnLoop()
{
	AbilityComp->PlayAbilityMontage(LoopMontage);
}

int UAbility::GetManaCost() const
{
	return ManaCost;
}

TEnumAsByte<ECollisionChannel> UAbility::GetAbilityCollisionChannel()
{
	return AbilityEffectChannel;
}

bool UAbility::ConsumeMana(int amount)
{
	return AbilityComp->ConsumeMana(amount);
}


