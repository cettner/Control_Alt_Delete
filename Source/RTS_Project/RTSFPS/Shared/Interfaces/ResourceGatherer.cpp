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


bool IResourceGatherer::CanCarryMore(TSubclassOf<AResource> ResourceClass) const
{
	return true;
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