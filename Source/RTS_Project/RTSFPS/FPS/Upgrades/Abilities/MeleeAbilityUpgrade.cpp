// Fill out your copyright notice in the Description page of Project Settings.


#include "MeleeAbilityUpgrade.h"

bool UMeleeAbilityUpgrade::ApplyUpgrade(UObject* ToUpgrade, const uint32 OldRank, const uint32 NewRank, const bool HasAuthority, const bool IsLocal) const
{
	bool retval = Super::ApplyUpgrade(ToUpgrade, OldRank, NewRank, HasAuthority, IsLocal);
	if (HasAuthority)
	{
		UMeleeAbility* meleeability = CastChecked<UMeleeAbility>(ToUpgrade);

		const uint32 currenthitcount = meleeability->GetMaxActortHitCount();
		const uint32 currentdamage = meleeability->GetBaseDamage();
		const int32 rankdiff = static_cast<int32>(NewRank) - static_cast<int32>(OldRank);

		const int32 totalhitchange = rankdiff * MultiHitEnhancement;
		const int32 totaldamagechange = rankdiff * DamageEnhancement;

		const int32 newhitcount = currenthitcount + totalhitchange;
		const int32 newdamage = currentdamage + totaldamagechange;

		meleeability->SetMaxActorHitCount(newhitcount);
		meleeability->SetBaseDamage(newdamage);

		retval = true;
	}
	else
	{
		retval = true;
	}

	return retval;
}

TSubclassOf<UAbility> UMeleeAbilityUpgrade::GetAbilityClass() const
{
	return AbilityClass;
}
