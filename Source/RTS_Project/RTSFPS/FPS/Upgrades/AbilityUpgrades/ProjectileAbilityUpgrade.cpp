// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileAbilityUpgrade.h"
#include "RTS_Project/RTSFPS/GameSystems/AbilitySystem/Interfaces/AbilityUserInterface.h"
#include "RTS_Project/RTSFPS/GameSystems/AbilitySystem/Abilities/ProjectileAbility.h"

void UProjectileAbilityUpgrade::ApplyUpgrade(UObject * ToUpgrade) const
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

			const float currentdamage = ability->GetProjectileDamage();
			const float newdamage = currentdamage + DamageIncrement;
			ability->SetProjectileDamage(newdamage);
		}
	}

}