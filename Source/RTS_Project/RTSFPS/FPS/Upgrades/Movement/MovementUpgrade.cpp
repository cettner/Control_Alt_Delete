// Fill out your copyright notice in the Description page of Project Settings.


#include "MovementUpgrade.h"

bool UMovementUpgrade::ApplyUpgrade(UObject* ToUpgrade, const uint32 OldRank, const uint32 NewRank, const bool HasAuthority, const bool IsLocal) const
{
	bool retval = false;
	if (HasAuthority)
	{
		UCharacterMovementComponent* movecomp = CastChecked<UCharacterMovementComponent>(ToUpgrade);
		const float newwalkspeed = EnhanceFloat(movecomp->MaxWalkSpeed, BaseMovementEnhancment, OldRank, NewRank);
		movecomp->MaxWalkSpeed = newwalkspeed;
		retval = true;
	}
	else
	{
		retval = true;
	}

	return retval;
}
