// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSAbilityUpgrade.h"
#include "RTS_Project/RTSFPS/GameSystems/AbilitySystem/Interfaces/AbilityUserInterface.h"
#include "RTS_Project/RTSFPS/GameSystems/UpgradeSystem/Interfaces/UpgradableInterface.h"





bool URTSAbilityUpgrade::CanUpgrade(IUpgradableInterface * TestUpgrade) const
{
	
	bool retval = Super::CanUpgrade(TestUpgrade);
	bool hasability = false;
	if (retval == true)
	{
		IAbilityUserInterface * abilityuser  = Cast<IAbilityUserInterface>(TestUpgrade);
		if (abilityuser == nullptr)
		{
			retval = false;
		}
		else
		{
			for (int i = 0; i < TargetAbilities.Num(); i++)
			{
				const bool abilitycheck = (abilityuser->GetAbilitiesByClass(TargetAbilities[i]).Num() > 0);
				hasability |= abilitycheck;
			}
		}
	}
	retval &= hasability;
	return retval;
}

TArray<TWeakObjectPtr<UAbility>> URTSAbilityUpgrade::GetTargetAbilitesFromUser(IAbilityUserInterface * AbilityUser) const
{
	IAbilityUserInterface * abilityuser = Cast<IAbilityUserInterface>(AbilityUser);
	TArray<TWeakObjectPtr<UAbility>> retval = TArray<TWeakObjectPtr<UAbility>>();

	for (int i = 0; i < TargetAbilities.Num(); i++)
	{
		retval.Append(AbilityUser->GetAbilitiesByClass(TargetAbilities[i]));
	}

	return retval;
}
