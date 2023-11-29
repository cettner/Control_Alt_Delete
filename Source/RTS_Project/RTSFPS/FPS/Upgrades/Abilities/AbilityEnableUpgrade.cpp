// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityEnableUpgrade.h"

UAbilityEnableUpgrade::UAbilityEnableUpgrade()
{
	MaxRank = 1U;
}

bool UAbilityEnableUpgrade::ApplyUpgrade(UObject* ToUpgrade, const uint32 OldRank, const uint32 NewRank, const bool HasAuthority, const bool IsLocal) const
{	
	bool retval = false;
	if (HasAuthority)
	{
		IAbilityUserInterface* abilityuser = CastChecked<IAbilityUserInterface>(ToUpgrade);
		int32 installcheck = 0U;

		if (NewRank > UPGRADE_UNLEARNED)
		{
			for (const TSubclassOf<UAbility>& abilitytoenable : TargetAbilites)
			{
				abilityuser->EnableAbility(abilitytoenable);
				installcheck++;
			}
		}
		else
		{
			for (const TSubclassOf<UAbility>& abilitytoenable : TargetAbilites)
			{
				if (abilityuser->DisableAbility(abilitytoenable))
				{
					installcheck++;
				}
			}
		}

		retval = installcheck == TargetAbilites.Num();
	}
	else
	{
		retval = true;
	}

	return retval;
}
