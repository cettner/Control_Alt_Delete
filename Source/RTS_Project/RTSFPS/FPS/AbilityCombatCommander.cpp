// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityCombatCommander.h"

void AAbilityCombatCommander::OnReadyNotify()
{
	IAbilityUserInterface * abilityweapon = Cast<IAbilityUserInterface>(CurrentWeapon);

	if (abilityweapon != nullptr)
	{
		abilityweapon->OnReadyNotify();
	}
}

void AAbilityCombatCommander::OnLoopNotify()
{
	IAbilityUserInterface * abilityweapon = Cast<IAbilityUserInterface>(CurrentWeapon);

	if (abilityweapon != nullptr)
	{
		abilityweapon->OnLoopNotify();
	}
}

void AAbilityCombatCommander::OnEffectNotify()
{
	IAbilityUserInterface * abilityweapon = Cast<IAbilityUserInterface>(CurrentWeapon);

	if (abilityweapon != nullptr)
	{
		abilityweapon->OnEffectNotify();
	}
}

void AAbilityCombatCommander::OnEndNotify()
{
	IAbilityUserInterface * abilityweapon = Cast<IAbilityUserInterface>(CurrentWeapon);

	if (abilityweapon != nullptr)
	{
		abilityweapon->OnEndNotify();
	}
}
