// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileAbilityUpgrade.h"

bool UProjectileAbilityUpgrade::ApplyUpgrade(UObject* ToUpgrade, const uint32 OldRank, const uint32 NewRank, const bool HasAuthority, const bool IsLocal) const
{
	bool retval = Super::ApplyUpgrade(ToUpgrade, OldRank, NewRank, HasAuthority, IsLocal);
	if (HasAuthority)
	{
		UProjectileAbility* projectileability = CastChecked<UProjectileAbility>(ToUpgrade);
		projectileability->GetProjectileSpeed();

		const uint32 currentspeed = projectileability->GetProjectileSpeed();
		const float currentdamage = projectileability->GetProjectileDamage();
		const int32 rankdiff = static_cast<int32>(NewRank) - static_cast<int32>(OldRank);

		const float totalspeedchange = rankdiff * ProjectileSpeedEnhancement;
		const float totaldamagechange = rankdiff * BaseDamageEnhancement;

		const float newspeed = currentspeed + totalspeedchange;
		const float newdamage = currentdamage + totaldamagechange;

		projectileability->SetProjectileSpeed(newspeed);
		projectileability->SetProjectileDamage(newdamage);
		retval = true;
	}
	else
	{
		retval = true;
	}

	return retval;
}

TSubclassOf<UAbility> UProjectileAbilityUpgrade::GetAbilityClass() const
{
	return AbilityClass;
}
