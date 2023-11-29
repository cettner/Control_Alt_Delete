// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityEnableUpgrade.h"

UAbilityEnableUpgrade::UAbilityEnableUpgrade()
{
	MaxRank = 1U;
}

bool UAbilityEnableUpgrade::ApplyUpgrade(UObject* ToUpgrade, const uint32 OldRank, const uint32 NewRank, const bool HasAuthority, const bool IsLocal) const
{
	IAbilityUserInterface * abilityuser = CastChecked<IAbilityUserInterface>(ToUpgrade);
	bool retval = false;
	if (NewRank > UPGRADE_UNLEARNED)
	{
		for (const TSubclassOf<UAbility>& abilitytoenable : TargetAbilites)
		{
			abilityuser->EnableAbility(abilitytoenable);
			retval = true;
		}
	}
	else
	{
		for (const TSubclassOf<UAbility>& abilitytoenable : TargetAbilites)
		{
			retval &= abilityuser->DisableAbility(abilitytoenable);
		}
	}

	return retval;
}
