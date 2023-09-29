// Fill out your copyright notice in the Description page of Project Settings.


#include "ResourceGathererComponent.h"

#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UResourceGathererComponent::UResourceGathererComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	PrimaryComponentTick.bStartWithTickEnabled = false;

	StartingResources = CreateDefaultSubobject<UResourceData>(TEXT("StartingResources"));
}

FOnResourceValueChangedDelegate& UResourceGathererComponent::BindResourceValueChangedEvent(const TSubclassOf<UResource> InResourceType)
{
	if (FOnResourceValueChangedDelegate* founddelegate = ResourceDelegates.Find(InResourceType))
	{
		return *founddelegate;
	}
	else
	{
		FOnResourceValueChangedDelegate newdelegate = FOnResourceValueChangedDelegate();
		ResourceDelegates.Emplace(InResourceType, newdelegate);

		return *ResourceDelegates.Find(InResourceType);
	}
}

void UResourceGathererComponent::AddResource(TSubclassOf<UResource> ResourceClass, int amount)
{
	const UResource* resourcecdo = ResourceClass.GetDefaultObject();
	const int* slotptr = HeldResources.Find(ResourceClass);

	const uint32 oldvalue = static_cast<uint32>(*slotptr);
	checkf(slotptr, TEXT("UResourceGathererComponent::AddResource, Failed to add Resource because it wasnt supported"));
	const uint32 newvalue = static_cast<uint32>(*slotptr);

	HeldResources.Increment(ResourceClass, amount);

	if (resourcecdo->IsWeightedResource())
	{
		CurrentWeight += resourcecdo->GetResourceWeight() * amount;
	}

	FOnResourceValueChangedDelegate* broadcast = ResourceDelegates.Find(ResourceClass);

	if (broadcast != nullptr)
	{
		(*broadcast).Broadcast(ResourceClass, oldvalue, newvalue, TScriptInterface<IResourceGatherer>(GetOwner()));
	}
}

// TODO: Make this say DecrementResource
bool UResourceGathererComponent::RemoveResource(const TSubclassOf<UResource> ResourceClass, int amount)
{
	const UResource* resourcecdo = ResourceClass.GetDefaultObject();
	const int* slotptr = HeldResources.Find(ResourceClass);
	checkf(slotptr, TEXT("UResourceGathererComponent::RemoveResource, Failed to remove Resource because it wasnt supported"));

	const uint32 oldvalue = static_cast<uint32>(*slotptr);
	bool retval = HeldResources.Decrement(ResourceClass, amount);
	const uint32 newvalue = static_cast<uint32>(*slotptr);
	
	if (resourcecdo->IsWeightedResource())
	{
		CurrentWeight -= resourcecdo->GetResourceWeight() * amount;
	}

	FOnResourceValueChangedDelegate* broadcast = ResourceDelegates.Find(ResourceClass);

	if (broadcast != nullptr)
	{
		(*broadcast).Broadcast(ResourceClass, oldvalue, newvalue, TScriptInterface<IResourceGatherer>(GetOwner()));
	}

	return retval;
}

FReplicationResourceMap UResourceGathererComponent::GetAllHeldResources() const
{
	//TODO: Make this return a HeldResources 
	
	return HeldResources;
}

uint32 UResourceGathererComponent::GetCurrentWeight() const
{
	return CurrentWeight;
}

uint32 UResourceGathererComponent::GetMaxWeight() const
{
	return MaxWeight;
}

void UResourceGathererComponent::SetResourceDiscreteMaximum(const TSubclassOf<UResource> InResourceClass, const uint32 InAmount)
{
	ResourceMaximums.Emplace(InResourceClass, InAmount);
}

void UResourceGathererComponent::SetResourceDiscreteMinimum(const TSubclassOf<UResource> InResourceClass, const uint32 InAmount)
{

}

uint32 UResourceGathererComponent::GetResourceDiscreteMaximum(const TSubclassOf<UResource> ResourceClass) const
{
	const int* maxptr = ResourceMaximums.Find(ResourceClass);
	checkf(maxptr, TEXT(" UResourceGathererComponent::GetResourceDiscreteMaximum failed to obtain maximum for Resource"));
	const uint32 retval = static_cast<uint32>(*maxptr);
	return retval;
}

uint32 UResourceGathererComponent::GetResourceDiscreteMinimum(const TSubclassOf<UResource> ResourceClass) const
{
	return 0U;
}

void UResourceGathererComponent::RecalculateWeight()
{
	CurrentWeight = 0U;
	for (int i = 0; i < HeldResources.Num(); i++)
	{
		const UResource * resourcecdo = HeldResources[i].Key.GetDefaultObject();
		if (resourcecdo->IsWeightedResource())
		{
			CurrentWeight += resourcecdo->GetResourceWeight() * HeldResources[i].Value;
		}
	}
}

// Anthonys cool code club ------------------------------------------------------
void UResourceGathererComponent::Emplace(const TSubclassOf<UResource> InKey, const uint32 InValue)
{
	Values.Emplace(InValue);
	Keys.Emplace(InKey);
}

bool UResourceGathererComponent::Remove(TSubclassOf<UResource> Key)
{
	UE_LOG(LogTemp, Fatal, TEXT("UResourceGathererComponent::Remove not implemented"));
	return false;
}

bool UResourceGathererComponent::IncOrDec(TSubclassOf<UResource> Key, uint32 Value, bool Increment)
{
	bool retVal = false;

	checkf(Key, TEXT("UResourceGathererComponent::IncOrDec, KEY is NULL"));

	if (Value == 0) {
		// No need to do anything if adding/subtracting zero
		retVal = true;
	}
	else if (Value > 0) {
		const int index = Keys.IndexOfByKey(Key);

		checkf(index == INDEX_NONE, TEXT("UResourceGathererComponent::IncOrDec, \
										  KEY wasn't found in Keys"));

		if (Increment == true) {
			Values[index] += Value;
		}
		else {
			Values[index] -= Value;
		}

		retVal = true;
	}
	return false;
}

bool UResourceGathererComponent::Increment(TSubclassOf<UResource> Key, uint32 Value)
{
	return IncOrDec(Key, Value);
	
}

bool UResourceGathererComponent::Decrement(TSubclassOf<UResource> Key, uint32 Value)
{
	return IncOrDec(Key, Value,false);
}


// END ----


void UResourceGathererComponent::OnRep_HeldResources()
{
	RecalculateWeight();
}

void UResourceGathererComponent::OnRegister()
{
	Super::OnRegister();
	
	for (TPair<TSubclassOf<UResource>, FResourceConfigData> resourceconfig : StartingResources->GetResourceConfig())
	{
		const TSubclassOf<UResource> resourceclass = resourceconfig.Key;
		const FResourceConfigData configdata = resourceconfig.Value;
		HeldResources.Emplace(resourceclass,configdata.StartingValue);
		SetResourceDiscreteMaximum(resourceclass, configdata.Max);
		SetResourceDiscreteMinimum(resourceclass, configdata.Min);
	}
	RecalculateWeight();
}

void UResourceGathererComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	//TODO: Might need to add the values array to this 
	DOREPLIFETIME(UResourceGathererComponent, HeldResources);
	DOREPLIFETIME(UResourceGathererComponent, ResourceMaximums);
	DOREPLIFETIME(UResourceGathererComponent, MaxWeight);
}
