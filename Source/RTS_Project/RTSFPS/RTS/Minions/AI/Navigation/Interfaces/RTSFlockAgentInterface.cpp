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

void IRTSFlockAgentInterface::ApplySeperation()
{
	URTSFlock* flock = GetFlock();
	SeperationForce = FVector::ZeroVector;

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

				SeperationForce += direction / distance;
			}
		}
	}
}

void IRTSFlockAgentInterface::ApplyAlignment()
{
	URTSFlock* flock = GetFlock();
	SeperationForce = FVector::ZeroVector;

	if (flock != nullptr)
	{
		const TArray<IRTSFlockAgentInterface*> flockmembers = flock->GetAgents();

		for (int i = 0; i < flockmembers.Num(); i++)
		{
			const IRTSFlockAgentInterface* otheragent = flockmembers[i];
			if (flockmembers[i] != this)
			{
				AlignmentForce += otheragent->GetFlockAgentDirection();
			}
		}

		AlignmentForce /= flockmembers.Num();
		AlignmentForce.Normalize();
	}
}

void IRTSFlockAgentInterface::ApplyCohesion()
{
	CohesionForce = FVector::ZeroVector;
	URTSFlock* flock = GetFlock();

	if (flock != nullptr)
	{
		const TArray<IRTSFlockAgentInterface*> flockmembers = flock->GetAgents();

		for (int i = 0; i < flockmembers.Num(); i++)
		{
			const IRTSFlockAgentInterface* otheragent = flockmembers[i];
			if (flockmembers[i] != this)
			{
				CohesionForce += otheragent->GetFlockAgentLocation();
			}
		}

		CohesionForce /= flockmembers.Num();

		/*Calculate the direction towards the "center of mass" of the flock*/
		CohesionForce = (CohesionForce - GetFlockAgentDirection()).GetSafeNormal();
	}
}

void IRTSFlockAgentInterface::ApplyObstacleAvoidance()
{
}
