// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../Resource.h"

#include "ResourceGatherer.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UResourceGatherer : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RTS_PROJECT_API IResourceGatherer
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void AddResource(TSubclassOf<AResource> ResourceClass, int amount);
	virtual bool RemoveResource(const TSubclassOf<AResource> ResourceClass, int amount);
	virtual void AddResource(const FReplicationResourceMap InResourceMap);
	virtual bool RemoveResource(const FReplicationResourceMap InResourceMap);
	virtual bool TransferResourceFromSource(IResourceGatherer* InDonar);
	virtual bool TransferResourceFromSource(IResourceGatherer* InDonar, const FReplicationResourceMap InAmounttoTransfer);
	virtual bool TransferResourceFromSource(IResourceGatherer* InDonar, const TSubclassOf<AResource> ResourceType, const int InAmountToTransfer);

	virtual bool HasResource(const TSubclassOf<AResource> ResourceClass, const uint32 amount = 1U) const;
	virtual bool HasResource(const FReplicationResourceMap InResourceMap) const;

	/*Returns whether the gatherer supports carrying the Resource type 
	Note: This should return true even if the gatherer is full, use CanCarryMore to determine if you can fit the desired amount*/
	virtual bool CanCarryResource(const TSubclassOf<AResource> InResource) const;

	/*Returns whether the gatherer can carry at least one more instance of the specified Resource.*/
	virtual bool CanCarryMore(TSubclassOf<AResource> ResourceClass, uint32 InNumtoCarry = 1U) const;
	/*Returns the number of resources of the specified class that can be carried until full*/
	virtual uint32 GetResourceTillFull(TSubclassOf<AResource> ResourceClass) const;
	virtual bool CanCarryAllResources(const FReplicationResourceMap InResourcestoCarry) const;
	virtual bool DropsResourceOnDeath() const;

	/*Returns a Map of All resources Carried by the instance*/
	virtual FReplicationResourceMap GetAllHeldResources() const;

	/*Returns the amount held by the unit of the particular type of Resource*/
	virtual uint32 GetHeldResource(TSubclassOf<AResource> ResourceClass) const;
	virtual uint32 GetCurrentWeight() const;
	virtual uint32 GetMaxWeight() const;
};
