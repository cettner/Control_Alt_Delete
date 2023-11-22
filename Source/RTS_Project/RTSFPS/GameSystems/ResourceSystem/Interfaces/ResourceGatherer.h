// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../Resource.h"

#include "ResourceGatherer.generated.h"


DECLARE_MULTICAST_DELEGATE_FourParams(FOnResourceValueChangedDelegate, const TSubclassOf<UResource>, const uint32/*old value*/, const uint32 /*new value*/, TScriptInterface<class IResourceGatherer>);
DECLARE_DELEGATE_RetVal(int32, FGetResource);

USTRUCT()
struct FResourceRegenEventConfig
{
	GENERATED_USTRUCT_BODY()

		friend class UResourceGathererComponent;

public:
	int32 GetRegenAmount() const
	{
		int32 retval = FlatRegenAmount;
		if (RegenAmountHandler.IsBound())
		{
			retval = RegenAmountHandler.Execute();
		}

		return retval;
	}

public:
	UPROPERTY(EditDefaultsOnly)
		float TickRate = .5f;

	/*Regen amount per tick*/
	UPROPERTY(EditDefaultsOnly)
		int32 FlatRegenAmount = 0;

	UPROPERTY(EditDefaultsOnly)
		/*Whether the regen should start immediatly, this might disabled if you want to configure RegenAmountHandler before play start*/
		bool bEnabledAtStart = true;

	/*Optionally allows for an external source to supply the amount of regen provided per tick*/
	FGetResource RegenAmountHandler = FGetResource();

protected:
	/*Runtime Specifies the function to execute when the timer elapses*/
	FTimerDelegate TimerDelegate = FTimerDelegate();

	/*Allows cancellation of the regen timer*/
	FTimerHandle TimerHandle = FTimerHandle();

	TSubclassOf<UResource> ResourceClass = nullptr;
};



// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UResourceGatherer : public UInterface
{
	GENERATED_BODY()
};


class RTS_PROJECT_API IResourceGatherer
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void AddResource(TSubclassOf<UResource> ResourceClass, int amount);
	virtual bool RemoveResource(const TSubclassOf<UResource> ResourceClass, int amount);
	virtual void AddResource(const FReplicationResourceMap InResourceMap);
	virtual bool RemoveResource(const FReplicationResourceMap InResourceMap);
	virtual bool TransferResourceFromSource(IResourceGatherer* InDonar);
	virtual bool TransferResourceFromSource(IResourceGatherer* InDonar, const FReplicationResourceMap InAmounttoTransfer);
	virtual bool TransferResourceFromSource(IResourceGatherer* InDonar, const TSubclassOf<UResource> ResourceType, const int InAmountToTransfer);

public:
	virtual FOnResourceValueChangedDelegate& BindResourceValueChangedEvent(const TSubclassOf<UResource> InResourceType) PURE_VIRTUAL(IResourceGatherer::BindResourceValueChangedEvent, return *new FOnResourceValueChangedDelegate(););
	virtual void AddResourceRegenEvent(FResourceRegenEventConfig InResourceConfig, const TSubclassOf<UResource>& InResourceClass);
	virtual bool ClearResourceRegenEvent(const TSubclassOf<UResource>& InResourceClass);
	virtual uint32 ClearAllResourceRegenEvents();
	virtual const FResourceRegenEventConfig* GetCurrentRegenEventConfig(const TSubclassOf<UResource>& InResourceClass) const;

	/*Returns Whether the gatherer has the specified number of units of a particular resource type*/
	virtual bool HasResource(const TSubclassOf<UResource> ResourceClass, const uint32 amount = 1U) const;
	virtual bool HasResource(const FReplicationResourceMap& InResourceMap) const;

	virtual uint32 GetResourceMaximum(const TSubclassOf<UResource> ResourceClass) const;
	virtual uint32 GetResourceMinimum(const TSubclassOf<UResource> ResourceClass) const;

	virtual void SetResourceDiscreteMaximum(const TSubclassOf<UResource> InResourceClass, const uint32 InAmount);
	virtual void SetResourceDiscreteMinimum(const TSubclassOf<UResource> InResourceClass, const uint32 InAmount);

	/*Returns whether the gatherer supports carrying the Resource type 
	Note: This should return true even if the gatherer is full, use CanCarryMore to determine if you can fit the desired amount*/
	virtual bool CanCarryResource(const TSubclassOf<UResource> InResource) const;
	/*Returns the subclasses of resource that the gatherer can carry (regardless of capacity)*/
	virtual const TArray<TSubclassOf<UResource>> GetSupportedResources() const;

	/*Returns whether the gatherer can carry at least one more instance of the specified Resource.*/
	virtual bool CanCarryMore(TSubclassOf<UResource> ResourceClass, uint32 InNumtoCarry = 1U) const;
	/*Returns the number of resources of the specified class that can be carried until full*/
	virtual uint32 GetResourceTillFull(TSubclassOf<UResource> ResourceClass) const;
	virtual bool CanCarryAllResources(const FReplicationResourceMap InResourcestoCarry) const;

	/*Returns a Map of All resources Carried by the instance*/
	virtual FReplicationResourceMap GetAllHeldResources() const;
	virtual FReplicationResourceMap GetAllWeightedResources() const;
	virtual FReplicationResourceMap GetAllDiscreteResources() const;

	/*Returns the amount held by the unit of the particular type of Resource*/
	virtual bool GetHeldResource(TSubclassOf<UResource> ResourceClass, uint32& OutAmount) const;
	virtual uint32 GetCurrentWeight() const;
	virtual uint32 GetMaxWeight() const;


protected:
	virtual uint32 GetResourceDiscreteMaximum(const TSubclassOf<UResource> InResourceClass) const;
	virtual uint32 GetResourceDiscreteMinimum(const TSubclassOf<UResource> InResourceClass) const;
};
