// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityUpgrade.h"

TSubclassOf<UAbility> UAbilityUpgrade::GetAbilityClass() const
{
	return nullptr;
}

bool UAbilityUpgrade::AdjustUpgradeCost(UAbility* InAbility, const uint32 InOldRank, const uint32 InNewRank) const
{
	bool retval = true;
	/*Get the old value and modify it*/
	FReplicationResourceMap newcost = InAbility->GetAbilityCost();
	const int32 rankdiff = static_cast<int32>(InNewRank) - static_cast<int32>(InOldRank);

	for (int i = 0; i < BaseCostReduction.Num(); i++)
	{
		const int32 currentvalue = *newcost.Find(BaseCostReduction[i].Key);
		const int32 basevalue = BaseCostReduction[i].Value;

		const uint32 totalchange = FMath::Abs(rankdiff * basevalue);

		if (rankdiff > 0)
		{
			/*In general if we're increasing the rank, we're decreasing the cost*/
			retval &= newcost.Decrement(BaseCostReduction[i].Key, totalchange);
		}
		else
		{
			retval &= newcost.Increment(BaseCostReduction[i].Key, totalchange);
		}
	}

	InAbility->SetAbilityCost(newcost);

	return retval;
}

bool UAbilityUpgrade::ApplyUpgrade(UObject* ToUpgrade, const uint32 OldRank, const uint32 NewRank, const bool HasAuthority, const bool IsLocal) const
{
	bool retval = false;
	if (BaseCostReduction.Num() && HasAuthority)
	{
		UAbility* abilityptr = CastChecked<UAbility>(ToUpgrade);
		retval = AdjustUpgradeCost(abilityptr, OldRank, NewRank);
	}
	else
	{
		retval = true;
	}

	return retval;
}
