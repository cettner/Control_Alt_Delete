// Fill out your copyright notice in the Description page of Project Settings.


#include "ResourceGatherer.h"

// Add default functionality here for any IResourceGatherer functions that are not pure virtual.


/*Pure Virtual*/
void IResourceGatherer::AddResource(TSubclassOf<UResource> ResourceClass, int amount)
{

}

bool IResourceGatherer::RemoveResource(TSubclassOf<UResource> ResourceClass, int amount)
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

void IResourceGatherer::SetResourceDiscreteMaximum(const TSubclassOf<UResource> InResourceClass, const uint32 InAmount)
{
}

void IResourceGatherer::SetResourceDiscreteMinimum(const TSubclassOf<UResource> InResourceClass, const uint32 InAmount)
{
}

uint32 IResourceGatherer::GetResourceDiscreteMaximum(const TSubclassOf<UResource> ResourceClass) const
{
	return uint32();
}

uint32 IResourceGatherer::GetResourceDiscreteMinimum(const TSubclassOf<UResource> ResourceClass) const
{
	return uint32();
}

void IResourceGatherer::AddResourceRegenEvent(FResourceRegenEventConfig InResourceConfig, const TSubclassOf<UResource>& InResourceClass)
{
}

bool IResourceGatherer::ClearResourceRegenEvent(const TSubclassOf<UResource>& InResourceClass)
{
	return false;
}

uint32 IResourceGatherer::ClearAllResourceRegenEvents()
{
	return uint32();
}

const FResourceRegenEventConfig* IResourceGatherer::GetCurrentRegenEventConfig(const TSubclassOf<UResource>& InResourceClass) const
{
	return nullptr;
}

/****** Non Pure Virtual*****/
bool IResourceGatherer::GetHeldResource(TSubclassOf<UResource> ResourceClass, uint32& OutAmount) const
{
	const FReplicationResourceMap resources = GetAllHeldResources();
	const int* foundvalue = resources.Find(ResourceClass);
	const bool retval = foundvalue != nullptr;
	if (retval)
	{
		OutAmount = static_cast<uint32>(*foundvalue);
	}

	return retval;
}

bool IResourceGatherer::HasResource(const TSubclassOf<UResource> ResourceClass, const uint32 amount) const
{
	const FReplicationResourceMap resources = GetAllHeldResources();
	const int* carriedvalue = resources.Find(ResourceClass);
	bool retval = carriedvalue != nullptr;

	if (retval)
	{
		retval = *carriedvalue >= static_cast<int>(amount);
	}

	return retval;
}

FReplicationResourceMap IResourceGatherer::GetAllWeightedResources() const
{
	const FReplicationResourceMap allresources = GetAllHeldResources();
	FReplicationResourceMap retval = FReplicationResourceMap();
	for (int i = 0; i < allresources.Num(); i++)
	{
		const TSubclassOf<UResource> resourceclass = allresources[i].Key;
		const UResource* resourcecdo = resourceclass.GetDefaultObject();
		if (resourcecdo->IsWeightedResource())
		{
			retval.Emplace(resourceclass, allresources[i].Value);
		}
	}
	return retval;
}

FReplicationResourceMap IResourceGatherer::GetAllDiscreteResources() const
{
	const FReplicationResourceMap allresources = GetAllHeldResources();
	FReplicationResourceMap retval = FReplicationResourceMap();
	for (int i = 0; i < allresources.Num(); i++)
	{
		const TSubclassOf<UResource> resourceclass = allresources[i].Key;
		const UResource* resourcecdo = resourceclass.GetDefaultObject();
		if (!resourcecdo->IsWeightedResource())
		{
			retval.Emplace(resourceclass, allresources[i].Value);
		}
	}
	return retval;
}

bool IResourceGatherer::CanCarryResource(const TSubclassOf<UResource> InResource) const
{
	const FReplicationResourceMap resources = GetAllHeldResources();
	const bool retval = resources.Find(InResource) != nullptr;
	return retval;
}

const TArray<TSubclassOf<UResource>> IResourceGatherer::GetSupportedResources() const
{
	const FReplicationResourceMap resources = GetAllHeldResources();
	return resources.GetKeys();
}

bool IResourceGatherer::HasResource(const FReplicationResourceMap& InResourceMap) const
{
	bool retval = true;
	for (int i = 0; i < InResourceMap.Num(); i++)
	{
		retval &= HasResource(InResourceMap[i].Key, static_cast<uint32>(InResourceMap[i].Value));
	}

	return retval;
}

uint32 IResourceGatherer::GetResourceMaximum(const TSubclassOf<UResource> InResourceClass) const
{
	uint32 retval = 0xFFFFFFFF;
	const UResource* resourcecdo = InResourceClass.GetDefaultObject();
	if (resourcecdo->IsWeightedResource())
	{
		const uint32 maxweight = GetMaxWeight();
		const uint32 resourceweight = resourcecdo->GetResourceWeight();

		retval = maxweight / resourceweight;

	}
	else
	{
		retval = GetResourceDiscreteMaximum(InResourceClass);
	}

	return retval;
}

uint32 IResourceGatherer::GetResourceMinimum(const TSubclassOf<UResource> InResourceClass) const
{
	const UResource* resourcecdo = InResourceClass.GetDefaultObject();
	uint32 retval = 0U;
	if (!resourcecdo->IsWeightedResource())
	{
		retval = GetResourceDiscreteMinimum(InResourceClass);
	}
	return 0U;
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

bool IResourceGatherer::TransferResourceFromSource(IResourceGatherer* InDonar, const TSubclassOf<UResource> ResourceType, const int InAmountToTransfer)
{
	//todo, add check for if the transfer can be completed for the reciever
	const bool retval = InDonar->RemoveResource(ResourceType, InAmountToTransfer);

	if (retval == true)
	{
		AddResource(ResourceType, InAmountToTransfer);
	}

	return retval;
}

bool IResourceGatherer::CanCarryMore(TSubclassOf<UResource> InResourceClass, uint32 InNumtoCarry) const
{
	const UResource* resourcecdo = InResourceClass.GetDefaultObject();
	bool retval = false;

	if (resourcecdo->IsWeightedResource())
	{
		const uint32 resourceweight = resourcecdo->GetResourceWeight() * InNumtoCarry;
		const uint32 currentweight = GetCurrentWeight();
		const uint32 maxweight = GetMaxWeight();
		const uint32 estimatedweight = currentweight + resourceweight;

		retval = estimatedweight <= maxweight;
	}
	else
	{
		uint32 heldresource = 0U;
		const uint32 discretemax = GetResourceDiscreteMaximum(InResourceClass);
		GetHeldResource(InResourceClass, heldresource);
		const uint32 estimatedcount = heldresource + InNumtoCarry;
		retval = estimatedcount <= discretemax;
	}

	return retval;
}

uint32 IResourceGatherer::GetResourceTillFull(TSubclassOf<UResource> InResourceClass) const
{
	uint32 retval = 0U;
	const UResource* defaultresource = InResourceClass.GetDefaultObject();

	if (defaultresource->IsWeightedResource())
	{
		const uint32 singleresourceweight = defaultresource->GetResourceWeight();

		if (singleresourceweight == 0U)
		{
			retval = 0xFFFFFFFFU;
		}
		else
		{
			const uint32 currentweight = GetCurrentWeight();
			const uint32 maxweight = GetMaxWeight();
			if (currentweight < maxweight)
			{
				const uint32 availableweight = maxweight - currentweight;
				retval = availableweight / singleresourceweight;
			}
		}
	}
	else
	{
		uint32 heldresource = 0U;
		const uint32 discretemax = GetResourceDiscreteMaximum(InResourceClass);
		GetHeldResource(InResourceClass, heldresource);
		retval =  discretemax - heldresource;
	}

	return retval;
}

bool IResourceGatherer::CanCarryAllResources(const FReplicationResourceMap InResourcestoCarry) const
{
	bool retval = true;
	uint32 estimatedweight = GetCurrentWeight();
	
	for (int i = 0; i < InResourcestoCarry.Num(); i++)
	{
		const UResource * resourcecdo = InResourcestoCarry[i].Key.GetDefaultObject();
		if (resourcecdo->IsWeightedResource())
		{
			const uint32 resourceweight = resourcecdo->GetResourceWeight();
			const uint32 resourcecount = StaticCast<uint32, int32>(InResourcestoCarry[i].Value);
			estimatedweight += (resourceweight * resourcecount);
		}
		else
		{
			uint32 heldresource = 0U;
			const uint32 discretemax = GetResourceDiscreteMaximum(InResourcestoCarry[i].Key);
			GetHeldResource(InResourcestoCarry[i].Key, heldresource);
			const uint32 estimatedresource = heldresource + InResourcestoCarry[i].Value;
			const bool canholdresources = estimatedresource <= discretemax;
			retval &= canholdresources;
		}
	}


	if (estimatedweight >= GetMaxWeight())
	{
		retval = false;
	}

	return retval;
}


