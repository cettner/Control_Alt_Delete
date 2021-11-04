// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileAbilityUpgrade.h"
#include "RTS_Project/RTSFPS/GameSystems/AbilitySystem/Interfaces/AbilityUserInterface.h"
#include "RTS_Project/RTSFPS/GameSystems/UpgradeSystem/Interfaces/UpgradableInterface.h"
#include "RTS_Project/RTSFPS/GameSystems/AbilitySystem/Abilities/ProjectileAbility.h"

void UProjectileAbilityUpgrade::ApplyUpgrade(IUpgradableInterface * ToUpgrade) const
{
	IAbilityUserInterface * abilityuser = Cast<IAbilityUserInterface>(ToUpgrade);
	TArray<TWeakObjectPtr<UAbility>> abilities = GetTargetAbilitesFromUser(abilityuser);

	for (int i = 0; i < abilities.Num(); i++)
	{
		if (abilities[i].IsValid() == true)
		{
			UProjectileAbility * ability = Cast<UProjectileAbility>(abilities[i].Get());
			const float currentspeed = ability->GetProjectileSpeed();
			const float newspeed = currentspeed + VelocityIncrement;
			ability->SetProjectileSpeed(newspeed);
		}
	}

}