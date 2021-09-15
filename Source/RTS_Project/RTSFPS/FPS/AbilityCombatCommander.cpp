// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityCombatCommander.h"

void AAbilityCombatCommander::OnReadyNotify(UAbilityAnimNotify * CallingContext)
{
	IAbilityUserInterface * abilityweapon = Cast<IAbilityUserInterface>(CurrentWeapon);

	if (abilityweapon != nullptr)
	{
		abilityweapon->OnReadyNotify(CallingContext);
	}
}

void AAbilityCombatCommander::OnLoopNotify(UAbilityAnimNotify * CallingContext)
{
	IAbilityUserInterface * abilityweapon = Cast<IAbilityUserInterface>(CurrentWeapon);

	if (abilityweapon != nullptr)
	{
		abilityweapon->OnLoopNotify(CallingContext);
	}
}

void AAbilityCombatCommander::OnEffectNotify(UAbilityAnimNotify * CallingContext)
{
	IAbilityUserInterface * abilityweapon = Cast<IAbilityUserInterface>(CurrentWeapon);

	if (abilityweapon != nullptr)
	{
		abilityweapon->OnEffectNotify(CallingContext);
	}
}

void AAbilityCombatCommander::OnEndNotify(UAbilityAnimNotify * CallingContext)
{
	IAbilityUserInterface * abilityweapon = Cast<IAbilityUserInterface>(CurrentWeapon);

	if (abilityweapon != nullptr)
	{
		abilityweapon->OnEndNotify(CallingContext);
	}
}
