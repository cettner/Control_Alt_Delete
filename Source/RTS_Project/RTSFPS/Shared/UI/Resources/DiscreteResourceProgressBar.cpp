// Fill out your copyright notice in the Description page of Project Settings.


#include "DiscreteResourceProgressBar.h"
#include "RTS_Project/RTSFPS/GameSystems/ResourceSystem/Interfaces/ResourceGatherer.h"

void UDiscreteResourceProgressBar::NativeConstruct()
{
	Super::NativeConstruct();
	PercentDelegate.BindUFunction(this, "UpdateResourceCapacity");
}

float UDiscreteResourceProgressBar::UpdateResourceCapacity()
{
	const IResourceGatherer* resourcecarrier = GetOwningPlayerPawn<IResourceGatherer>();
	float retval = 0.0f;

	if (resourcecarrier != nullptr)
	{
		const uint32 maxresource = resourcecarrier->GetResourceMaximum(TrackedResourceClass);
		uint32 heldresource = 0U;
		resourcecarrier->GetHeldResource(TrackedResourceClass, heldresource);
		retval = (float)heldresource / (float)maxresource;
	}



	return retval;
}
