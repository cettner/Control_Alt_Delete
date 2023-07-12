// Fill out your copyright notice in the Description page of Project Settings.


#include "FlockAgentInterface.h"
// Add default functionality here for any IRTSFlockAgentInterface functions that are not pure virtual.

const FObstacleTraceData FObstacleTraceData::DefaultObstacleTraceData
{
	FVector::ZeroVector,         // TraceOriginOffset
	200.0f,                              // TraceLength
	90.0f,								 // TraceArcSizeDegrees
	5,                                  // TraceCount
	ECollisionChannel::ECC_WorldDynamic  // TraceChannel
};


const FObstacleTraceData& IFlockAgentInterface::GetObstacleTraceData() const
{
	return FObstacleTraceData::DefaultObstacleTraceData;
}

const TSet<const UObject*> IFlockAgentInterface::GetFlockAgents() const
{
	return TSet<const UObject*>();
}

FVector IFlockAgentInterface::GetFlockAgentLocation() const
{
	return FVector();
}

FVector IFlockAgentInterface::GetFlockAgentDirection() const
{
	return FVector();
}

float IFlockAgentInterface::GetFlockAgentRadius() const
{
	return 0.0f;
}

bool IFlockAgentInterface::IsFlockMember(const AActor* InActor) const
{
	return false;
}

void IFlockAgentInterface::UpdateFlockParameters()
{
	FlockSeparation = CalcSeparation();
	FlockAlignment = CalcAlignment();
	FlockCohesion = CalcCohesion();
}

FVector IFlockAgentInterface::CalcSeparation()
{
	FVector retval = FVector::ZeroVector;
	const TSet<const UObject*> flockmembers = GetFlockAgents();

	for (const UObject* const& Elem : flockmembers)
	{
		const IFlockAgentInterface* otheragent = Cast<IFlockAgentInterface>(Elem);
		if (otheragent != this)
		{
			const FVector otherposition = otheragent->GetFlockAgentLocation();
			const FVector myposition = GetFlockAgentLocation();
			const FVector direction = (otherposition - myposition).GetSafeNormal();
			const float distance = FVector::Distance(otherposition, myposition);

			retval += direction / distance;
		}
	}

	return retval;
}

FVector IFlockAgentInterface::CalcAlignment()
{
	FVector retval = FVector::ZeroVector;

	const TSet<const UObject*> flockmembers = GetFlockAgents();

	for (const UObject* const& Elem : flockmembers)
	{
		const IFlockAgentInterface* otheragent = CastChecked<IFlockAgentInterface>(Elem);
			
		if (otheragent != this)
		{
			retval += otheragent->GetFlockAgentDirection();
		}
	}

	retval /= flockmembers.Num();
	retval.Normalize();

	return retval;
}

FVector IFlockAgentInterface::CalcCohesion()
{
	FVector retval = FVector::ZeroVector;
	const TSet<const UObject*> flockmembers = GetFlockAgents();
	
	if (flockmembers.Num())
	{
		for (const UObject* const& Elem : flockmembers)
		{
			const IFlockAgentInterface* otheragent = CastChecked<IFlockAgentInterface>(Elem);
			if (otheragent != this)
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

