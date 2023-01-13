// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSFlockAgentInterface.h"
#include "../RTSFlock.h"
// Add default functionality here for any IRTSFlockAgentInterface functions that are not pure virtual.

FVector IRTSFlockAgentInterface::GetFlockAgentLocation() const
{
	return FVector();
}

FVector IRTSFlockAgentInterface::GetFlockAgentVelocity() const
{
	return FVector();
}

FVector IRTSFlockAgentInterface::GetFlockAgentDirection() const
{
	return FVector();
}

FVector IRTSFlockAgentInterface::CalcSeparation()
{
	URTSFlock* flock = GetFlock();
	FVector retval = FVector::ZeroVector;

	if (flock != nullptr)
	{
		const TArray<IRTSFlockAgentInterface*> flockmembers = flock->GetAgents();

		for (int i = 0; i < flockmembers.Num(); i++)
		{
			const IRTSFlockAgentInterface* otheragent = flockmembers[i];
			if (flockmembers[i] != this)
			{
				const FVector otherposition = otheragent->GetFlockAgentLocation();
				const FVector myposition = GetFlockAgentLocation();
				const FVector direction = (otherposition - myposition).GetSafeNormal();
				const float distance = FVector::Distance(otherposition, myposition);

				retval += direction / distance;
			}
		}
	}
	return retval;
}

FVector IRTSFlockAgentInterface::CalcAlignment()
{
	URTSFlock* flock = GetFlock();
	FVector retval = FVector::ZeroVector;

	if (flock != nullptr)
	{
		const TArray<IRTSFlockAgentInterface*> flockmembers = flock->GetAgents();

		for (int i = 0; i < flockmembers.Num(); i++)
		{
			const IRTSFlockAgentInterface* otheragent = flockmembers[i];
			if (flockmembers[i] != this)
			{
				retval += otheragent->GetFlockAgentDirection();
			}
		}

		retval /= flockmembers.Num();
		retval.Normalize();
	}

	return retval;
}

FVector IRTSFlockAgentInterface::CalcCohesion()
{
	FVector retval = FVector::ZeroVector;
	URTSFlock* flock = GetFlock();

	if (flock != nullptr)
	{
		const TArray<IRTSFlockAgentInterface*> flockmembers = flock->GetAgents();

		for (int i = 0; i < flockmembers.Num(); i++)
		{
			const IRTSFlockAgentInterface* otheragent = flockmembers[i];
			if (flockmembers[i] != this)
			{
				retval += otheragent->GetFlockAgentLocation();
			}
		}

		retval /= flockmembers.Num();

		/*Calculate the direction towards the "center of mass" of the flock*/
		retval = (retval - GetFlockAgentDirection()).GetSafeNormal();
	}

	return retval;
}

