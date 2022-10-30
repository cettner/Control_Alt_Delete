// Fill out your copyright notice in the Description page of Project Settings.


#include "ResourceGatherer.h"

// Add default functionality here for any IResourceGatherer functions that are not pure virtual.

void IResourceGatherer::AddResource(TSubclassOf<AResource> ResourceClass, int amount)
{

}

bool IResourceGatherer::RemoveResource(TSubclassOf<AResource> ResourceClass, int amount)
{
	return false;
}

void IResourceGatherer::AddResource(const FReplicationResourceMap InResourceMap)
{
	for (int i = 0; i < InResourceMap.Num(); i++)
	{
		
		AddResource(InResourceMap[i].Key, InResourceMap[i].Value);
	}
}

bool IResourceGatherer::RemoveResource(const FReplicationResourceMap InResourceMap)
{
	bool retval = true;
	for (int i = 0; i < InResourceMap.Num(); i++)
	{
		retval &= RemoveResource(InResourceMap[i].Key, InResourceMap[i].Value);
	}
	return retval;
}

bool IResourceGatherer::TransferResourceFromSource(IResourceGatherer* InDonar)
{
	const FReplicationResourceMap  allresources = InDonar->GetAllHeldResources();

	//todo, add check for if the transfer can be completed for the reciever.
	const bool retval = InDonar->RemoveResource(allresources);

	if (retval == true)
	{
		AddResource(allresources);
	}

	return retval;
}

bool IResourceGatherer::TransferResourceFromSource(IResourceGatherer* InDonar, const FReplicationResourceMap InAmounttoTransfer)
{
	//todo, add check for if the transfer can be completed for the reciever
	const bool retval = InDonar->RemoveResource(InAmounttoTransfer);

	if (retval == true)
	{
		AddResource(InAmounttoTransfer);
	}

	return retval;
}

bool IResourceGatherer::TransferResourceFromSource(IResourceGatherer* InDonar, const TSubclassOf<AResource> ResourceType, const int InAmountToTransfer)
{
	//todo, add check for if the transfer can be completed for the reciever
	const bool retval = InDonar->RemoveResource(ResourceType, InAmountToTransfer);

	if (retval == true)
	{
		AddResource(ResourceType, InAmountToTransfer);
	}

	return retval;
}

bool IResourceGatherer::HasResource(const TSubclassOf<AResource> ResourceClass, const uint32 amount) const
{
	return false;
}

bool IResourceGatherer::HasResource(const FReplicationResourceMap InResourceMap) const
{
	bool retval = InResourceMap.Num() > 0;
	for (int i = 0; i < InResourceMap.Num(); i++)
	{
		retval &= HasResource(InResourceMap[i].Key, static_cast<uint32>(InResourceMap[i].Value));
	}

	return retval;
}

uint32 IResourceGatherer::CanCarryMore(TSubclassOf<AResource> ResourceClass) const
{
	return 0U;
}

bool IResourceGatherer::CanCarryAllResources(const FReplicationResourceMap InResourcestoCarry) const
{
	bool retval = true;
	uint32 estimatedweight = GetCurrentWeight();
	
	for (int i = 0; i < InResourcestoCarry.Num(); i++)
	{
		const AResource * resourcecdo = InResourcestoCarry[i].Key.GetDefaultObject();
		const uint32 resourceweight = resourcecdo->GetResourceWeight();
		const uint32 resourcecount = StaticCast<uint32,int32>(InResourcestoCarry[i].Value);
		estimatedweight += (resourceweight * resourcecount);
	}


	if (estimatedweight >= GetMaxWeight())
	{
		retval = false;
	}

	return retval;
}

uint32 IResourceGatherer::GetHeldResource(TSubclassOf<AResource> ResourceClass) const
{
	return 0U;
}

bool IResourceGatherer::DropsResourceOnDeath() const
{
	return false;
}

FReplicationResourceMap IResourceGatherer::GetAllHeldResources() const
{
	return FReplicationResourceMap();
}

uint32 IResourceGatherer::GetCurrentWeight() const
{
	return 0U;
}

uint32 IResourceGatherer::GetMaxWeight() const
{
	return 0xFFFFFFFF;
}