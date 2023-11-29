// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityCombatCommander.h"

#include "Net/UnrealNetwork.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/PlayerController.h"

AAbilityCombatCommander::AAbilityCombatCommander() : Super()
{
}

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

TArray<TWeakObjectPtr<UAbility>> AAbilityCombatCommander::GetAbilitiesByClass(const TSubclassOf<UAbility>& InAbilityClass) const
{
	IAbilityUserInterface * abilityweapon = Cast<IAbilityUserInterface>(CurrentWeapon);
	TArray<TWeakObjectPtr<UAbility>> retval = TArray<TWeakObjectPtr<UAbility>>();
	if (abilityweapon != nullptr)
	{
		retval = abilityweapon->GetAbilitiesByClass(InAbilityClass);
	}
	return retval;
}

TWeakObjectPtr<UAbility> AAbilityCombatCommander::GetFirstAbilityByClass(const TSubclassOf<UAbility>& InAbilityClass) const
{
	TWeakObjectPtr<UAbility> retval = nullptr;
	for (int32 i = 0; i < Inventory.Num(); i++)
	{
		if (IAbilityUserInterface* abilityweapon = Cast<IAbilityUserInterface>(Inventory[i]))
		{
			if (abilityweapon->SupportsAbility(InAbilityClass))
			{
				retval = abilityweapon->GetFirstAbilityByClass(InAbilityClass);
			}
		}
	}

	return retval;
}

void AAbilityCombatCommander::EnableAbility(const TSubclassOf<UAbility>& InAbilityClass)
{
	for (int32 i = 0; i < Inventory.Num(); i++)
	{
		if (IAbilityUserInterface * abilityweapon = Cast<IAbilityUserInterface>(Inventory[i]))
		{
			if (abilityweapon->SupportsAbility(InAbilityClass))
			{
				abilityweapon->EnableAbility(InAbilityClass);
			}
		}
	}
	IAbilityUserInterface* abilityweapon = Cast<IAbilityUserInterface>(CurrentWeapon);

	abilityweapon->EnableAbility(InAbilityClass);
}

bool AAbilityCombatCommander::DisableAbility(const TSubclassOf<UAbility>& InAbilityClass)
{
	bool retval = false;
	for (int32 i = 0; i < Inventory.Num(); i++)
	{
		if (IAbilityUserInterface* abilityweapon = Cast<IAbilityUserInterface>(Inventory[i]))
		{
			if (abilityweapon->SupportsAbility(InAbilityClass))
			{
				retval |= abilityweapon->DisableAbility(InAbilityClass);
			}
		}
	}

	return retval;
}

bool AAbilityCombatCommander::IsAbilityEnabled(const TSubclassOf<UAbility>& InAbilityClass) const
{
	const bool issupported = SupportsAbility(InAbilityClass);
	bool retval = false;
	
	if (issupported)
	{
		for (int32 i = 0; i < Inventory.Num(); i++)
		{
			if (IAbilityUserInterface* abilityweapon = Cast<IAbilityUserInterface>(Inventory[i]))
			{
				if (abilityweapon->IsAbilityEnabled(InAbilityClass))
				{
					retval = true;
					break;
				}
			}
		}
	}

	return retval;
}

TSet<TSubclassOf<UAbility>> AAbilityCombatCommander::GetSupportedAbilities() const
{
	TSet<TSubclassOf<UAbility>> retval = TSet<TSubclassOf<UAbility>>();
	for (int32 i = 0; i < Inventory.Num(); i++)
	{
		if (IAbilityUserInterface* abilityweapon = Cast<IAbilityUserInterface>(Inventory[i]))
		{
			retval.Append(abilityweapon->GetSupportedAbilities());
		}
	}
	
	return retval;
}

bool AAbilityCombatCommander::SupportsAbility(const TSubclassOf<UAbility>& InAbilityclass) const
{
	bool retval = false;
	for (int32 i = 0; i < Inventory.Num(); i++)
	{
		if (IAbilityUserInterface* abilityweapon = Cast<IAbilityUserInterface>(Inventory[i]))
		{
			if (abilityweapon->SupportsAbility(InAbilityclass))
			{
				retval = true;
				break;
			}
		}
	}
	return retval;
}

bool AAbilityCombatCommander::CanCastAbility(const TWeakObjectPtr<UAbility> TracingAbility) const
{
	FReplicationResourceMap abilitycost = TracingAbility->GetAbilityCost();

	bool retval = false;
	
	if (HasResource(abilitycost))
	{
		retval = true;
	}

	return retval;
}

bool AAbilityCombatCommander::SpendAbilityCost(const TWeakObjectPtr<UAbility> SpendingAbility)
{
	FReplicationResourceMap abilitycost = SpendingAbility->GetAbilityCost();
	bool retval = false;

	if (HasResource(abilitycost))
	{
		retval = IResourceGatherer::RemoveResource(abilitycost);
	}

	return retval;
}

void AAbilityCombatCommander::GrantExp(uint32 InExp)
{
	APlayerState* ps = GetPlayerState<APlayerState>();
	IExpAccumulatorInterface* expstate = Cast<IExpAccumulatorInterface>(ps);
	expstate->GrantExp(InExp);
}

