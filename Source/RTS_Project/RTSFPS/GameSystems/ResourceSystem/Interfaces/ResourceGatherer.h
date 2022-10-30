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

	virtual bool HasResource(const TSubclassOf<AResource> ResourceClass, const uint32 amount = 0U) const;
	virtual bool HasResource(const FReplicationResourceMap InResourceMap) const;

	/*Returns the count of the specified class that the gatherer can carry until the cap is achieved.*/
	virtual uint32 CanCarryMore(TSubclassOf<AResource> ResourceClass) const;
	virtual bool CanCarryAllResources(const FReplicationResourceMap InResourcestoCarry) const;
	virtual bool DropsResourceOnDeath() const;

	/*Returns a Map of All resources Carried by the instance*/
	virtual FReplicationResourceMap GetAllHeldResources() const;

	/*Returns the amount held by the unit of the particular type of Resource*/
	virtual uint32 GetHeldResource(TSubclassOf<AResource> ResourceClass) const;
	virtual uint32 GetCurrentWeight() const;
	virtual uint32 GetMaxWeight() const;
};
