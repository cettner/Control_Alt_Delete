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
	const uint32* slotptr = Find(ResourceClass);

	const uint32 oldvalue = *slotptr;
	checkf(slotptr, TEXT("UResourceGathererComponent::AddResource, Failed to add Resource because it wasnt supported"));
	
	const uint32 maxvalue = GetResourceMaximum(ResourceClass);
	const uint32 expectedvalue = oldvalue + amount;
	if (expectedvalue > maxvalue)
	{
		const uint32 maxpull = maxvalue - oldvalue;
		Increment(ResourceClass, maxpull);
	}
	else
	{
		Increment(ResourceClass, amount);
	}
	const uint32 newvalue = *slotptr;

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
	const uint32* resourceValuePtr = Find(ResourceClass);
	checkf(resourceValuePtr, TEXT("UResourceGathererComponent::RemoveResource, Failed to remove Resource because it wasnt supported"));
	
	const int32 minvalue = StaticCast<int32>(GetResourceMinimum(ResourceClass));
	const uint32 oldvalue = *resourceValuePtr;

	const int32 expectedvalue = oldvalue - amount;
	
	bool retval = false;
	if (expectedvalue < minvalue)
	{
		retval = Decrement(ResourceClass, oldvalue);
	}
	else
	{
		retval = Decrement(ResourceClass, amount);
	}

	const uint32 newvalue = *resourceValuePtr;
	
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

bool UResourceGathererComponent::SetResource(const TSubclassOf<UResource>& InResource, const uint32 InAmount)
{
	uint32 oldvalue = 0U;
	GetHeldResource(InResource, oldvalue);
	UResource* resourcecdo = InResource.GetDefaultObject();
	bool retval = true;

	if (InAmount != oldvalue)
	{
		const int* indexptr = ResourceToIndex.Find(InResource);
		checkf(indexptr, TEXT("UResourceGathererComponent::IncOrDec, key wasn't found in resource indicies"));

		const int index = *indexptr;
		const uint32 resourcemin = GetResourceMinimum(InResource);
		const uint32 resourcemax = GetResourceMaximum(InResource);
		const uint32 clampedvalue = FMath::Clamp(InAmount, resourcemin, resourcemax);

		Values[index] = clampedvalue;

		if (resourcecdo->IsWeightedResource())
		{
			RecalculateWeight();
		}
		
		FOnResourceValueChangedDelegate* broadcast = ResourceDelegates.Find(InResource);

		if (broadcast != nullptr)
		{
			(*broadcast).Broadcast(InResource, oldvalue, clampedvalue, TScriptInterface<IResourceGatherer>(GetOwner()));
		}

	}

	return retval;
}

FReplicationResourceMap UResourceGathererComponent::GetAllHeldResources() const
{
	FReplicationResourceMap HeldResources = FReplicationResourceMap();

	for (TPair<TSubclassOf<UResource>, int > keyValuePair : ResourceToIndex) 
	{
		HeldResources.Emplace(keyValuePair.Key, Values[keyValuePair.Value]);
	}

	return HeldResources;
}

FReplicationResourceMap UResourceGathererComponent::GetAllWeightedResources() const
{
	FReplicationResourceMap retval = FReplicationResourceMap();
	for (TPair<TSubclassOf<UResource>, int > keyValuePair : ResourceToIndex) 
	{
		const UResource* resourcecdo = keyValuePair.Key.GetDefaultObject();
		if (resourcecdo->IsWeightedResource())
		{
			retval.Emplace(keyValuePair.Key, Values[keyValuePair.Value]);
		}
	}
	return retval;
}

FReplicationResourceMap UResourceGathererComponent::GetAllDiscreteResources() const
{
	FReplicationResourceMap retval = FReplicationResourceMap();
	for (TPair<TSubclassOf<UResource>, int > keyValuePair : ResourceToIndex)
	{
		const UResource* resourcecdo = keyValuePair.Key.GetDefaultObject();
		if (!resourcecdo->IsWeightedResource())
		{
			retval.Emplace(keyValuePair.Key, Values[keyValuePair.Value]);
		}
	}
	return retval;
}

uint32 UResourceGathererComponent::GetCurrentWeight() const
{
	return CurrentWeight;
}

uint32 UResourceGathererComponent::GetMaxWeight() const
{
	return MaxWeight;
}

void UResourceGathererComponent::SetResourceDiscreteMaximum(const TSubclassOf<UResource> InResourceClass, uint32 InAmount, const EResourceBoundsAdjustment AdjustmentRules)
{
	if (!ResourceMaximums.Find(InResourceClass))
	{
		ResourceMaximums.Emplace(InResourceClass, InAmount);
	}
	else
	{
		const uint32 maxtochange = GetResourceMaximum(InResourceClass);
		const uint32 currentmin = GetResourceMinimum(InResourceClass);
		if (maxtochange == InAmount) return;

		if (InAmount < currentmin)
		{
			InAmount = currentmin;
			UE_LOG(LogTemp, Warning, TEXT("Warning UResourceGathererComponent::SetResourceDiscreteMaximum Requested Maximum was lower than the current Minimum, adjusting to minimum"))
		}

		const bool bincrementing = InAmount > maxtochange;
		ResourceMaximums.Emplace(InResourceClass, InAmount);

		if (AdjustmentRules != EResourceBoundsAdjustment::DONT_ADJUST)
		{
			uint32 currentvalue = 0U;
			checkf(GetHeldResource(InResourceClass, currentvalue), TEXT("UResourceGathererComponent::SetResourceDiscreteMaximum Resource to be adjusted not found"));

			if (AdjustmentRules == EResourceBoundsAdjustment::ADJUST_IF_OUT_OF_BOUNDS)
			{
				/*Set will ignore if the value is unchanged*/
				currentvalue = FMath::Clamp(currentvalue, currentmin, InAmount);
				SetResource(InResourceClass, currentvalue);
			}
			else if (AdjustmentRules == EResourceBoundsAdjustment::ADJUST_ON_INCREMENT_OR_OUT_OF_BOUNDS)
			{
				uint32 totalresourceamount = currentvalue;
				if (bincrementing)
				{
					/*we do the increment here so the change delegate is only triggered once*/
					const uint32 addedresources = InAmount - maxtochange;
					totalresourceamount += addedresources;
				}

				/*Set will ignore if the value is unchanged*/
				totalresourceamount = FMath::Clamp(totalresourceamount, currentmin, InAmount);
				SetResource(InResourceClass, totalresourceamount);
			}
			else if (AdjustmentRules == EResourceBoundsAdjustment::ALWAYS_ADJUST)
			{
				uint32 totalresourceamount = StaticCast<int32>(currentvalue);
				if (bincrementing)
				{
					/*we do the increment here so the change delegate is only triggered once*/
					const uint32 addedresources = InAmount - maxtochange;
					totalresourceamount += addedresources;
				}
				else
				{
					const int32 removedresources = StaticCast<int32>(maxtochange - InAmount);
					int32 estimatedvalue = StaticCast<int32>(totalresourceamount) - removedresources;
					if (estimatedvalue < StaticCast<int32>(currentmin))
					{
						estimatedvalue = StaticCast<int32>(currentmin);
					}
					totalresourceamount = StaticCast<uint32>(estimatedvalue);
				}

				/*Set will ignore if the value is unchanged*/
				totalresourceamount = FMath::Clamp(totalresourceamount, currentmin, InAmount);
				SetResource(InResourceClass, totalresourceamount);
			}
		}
	}


}

void UResourceGathererComponent::SetResourceDiscreteMinimum(const TSubclassOf<UResource> InResourceClass, const uint32 InAmount, const EResourceBoundsAdjustment AdjustmentRules)
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

	TArray<TSubclassOf<UResource>> keys = TArray<TSubclassOf<UResource>>();
	ResourceToIndex.GetKeys(keys);

	for (int i = 0; i < keys.Num(); i++)
	{
		const TSubclassOf<UResource> resourceKeyPtr = keys[i];
		const UResource* resourcecdo = resourceKeyPtr.GetDefaultObject();

		if (resourcecdo->IsWeightedResource())
		{
			// Need to get the index of the value from the Map
			const int* indexPtr = ResourceToIndex.Find(resourceKeyPtr);
			const int index = *indexPtr;

			CurrentWeight += resourcecdo->GetResourceWeight() * Values[index];
		}
	}
}

// Anthonys cool code club ------------------------------------------------------
void UResourceGathererComponent::Emplace(const TSubclassOf<UResource> InKey, const uint32 InValue)
{
	Values.Emplace(InValue);
	ResourceToIndex.Emplace(InKey,Values.Num()-1);
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

	if (Value == 0) 
	{
		// No need to do anything if adding/subtracting zero
		retVal = true;
	}
	else if (Value > 0) 
	{
		const int* indexPtr = ResourceToIndex.Find(Key);
		checkf(indexPtr, TEXT("UResourceGathererComponent::IncOrDec, key wasn't found in resource indicies"));

		const int index = *indexPtr;

		if (Increment == true) 
		{
			Values[index] += Value;
		}
		else 
		{
			Values[index] -= Value;
		}

		retVal = true;
	}
	return retVal;
}

bool UResourceGathererComponent::Increment(TSubclassOf<UResource> Key, uint32 Value)
{
	return IncOrDec(Key, Value);
}

bool UResourceGathererComponent::Decrement(TSubclassOf<UResource> Key, uint32 Value)
{
	return IncOrDec(Key, Value,false);
}

const uint32* UResourceGathererComponent::Find(TSubclassOf<UResource> Key) const
{
	bool retVal = false;

	const int* indexPtr = ResourceToIndex.Find(Key);
	checkf(indexPtr, TEXT("UResourceGathererComponent:: Key index not found"));

	const int index = *indexPtr;

	return &Values[index];
}

const int UResourceGathererComponent::Num() const
{
	return Values.Num();
}
// END ----

void UResourceGathererComponent::AddResourceRegenEvent(FResourceRegenEventConfig InResourceConfig, const TSubclassOf<UResource>& InResourceClass)
{
	if (FResourceRegenEventConfig* existingconfig = ActiveRegenEvents.Find(InResourceClass))
	{	
		existingconfig->FlatRegenAmount = InResourceConfig.FlatRegenAmount;
		existingconfig->RegenAmountHandler = InResourceConfig.RegenAmountHandler;
		existingconfig->TickRate = InResourceConfig.TickRate;

		GetWorld()->GetTimerManager().SetTimer(existingconfig->TimerHandle, existingconfig->TimerDelegate, existingconfig->TickRate, true);
	}
	else
	{
		FResourceRegenEventConfig& newconfig = ActiveRegenEvents.Emplace(InResourceClass, InResourceConfig);
		newconfig.TimerDelegate.BindUFunction(this, FName("HandleResourceRegenEvent"), InResourceClass);
		GetWorld()->GetTimerManager().SetTimer(newconfig.TimerHandle, newconfig.TimerDelegate, newconfig.TickRate, true);
	}
}

bool UResourceGathererComponent::ClearResourceRegenEvent(const TSubclassOf<UResource>& InResourceClass)
{
	bool retval = false;
	if (FResourceRegenEventConfig * eventconfig = ActiveRegenEvents.Find(InResourceClass))
	{
		GetWorld()->GetTimerManager().ClearTimer(eventconfig->TimerHandle);

		retval = ActiveRegenEvents.Remove(InResourceClass) > 0;
	}

	return retval;
}

uint32 UResourceGathererComponent::ClearAllResourceRegenEvents()
{
	uint32 retval = 0U;
	for (const TPair<TSubclassOf<UResource>,FResourceRegenEventConfig>& resource : ActiveRegenEvents)
	{
		if (ClearResourceRegenEvent(resource.Key))
		{
			retval++;
		}
	}

	return retval;
}

const FResourceRegenEventConfig* UResourceGathererComponent::GetCurrentRegenEventConfig(const TSubclassOf<UResource>& InResourceClass) const
{
	return ActiveRegenEvents.Find(InResourceClass);
}

void UResourceGathererComponent::HandleResourceRegenEvent(TSubclassOf<UResource> InEventKey)
{
	if (FResourceRegenEventConfig* eventconfigptr = ActiveRegenEvents.Find(InEventKey))
	{
		FResourceRegenEventConfig eventconfig = *eventconfigptr;
		const int32 amount = eventconfig.GetRegenAmount();

		if (amount > 0)
		{
			AddResource(InEventKey, amount);
		}
		else if (amount < 0)
		{
			const int32 positiveamount = -amount;
			uint32 value = static_cast<uint32>((positiveamount));
			RemoveResource(InEventKey, value);
		}
	}
}

void UResourceGathererComponent::OnRep_HeldResources()
{
	RecalculateWeight();
}

void UResourceGathererComponent::OnRegister()
{
	Super::OnRegister();
	
	for (const TPair<TSubclassOf<UResource>, FResourceConfigData> &resourceconfig : StartingResources->GetResourceConfig())
	{
		const TSubclassOf<UResource> resourceclass = resourceconfig.Key;
		const FResourceConfigData configdata = resourceconfig.Value;
		Emplace(resourceclass,configdata.StartingValue);
		SetResourceDiscreteMaximum(resourceclass, configdata.Max);
		SetResourceDiscreteMinimum(resourceclass, configdata.Min);
	}
	RecalculateWeight();
}

void UResourceGathererComponent::BeginPlay()
{
	Super::BeginPlay();

	if(GetOwner()->HasAuthority())
	{
		for (const TPair<TSubclassOf<UResource>, FResourceRegenEventConfig>& resourceconfig : StartingResources->GetReSourceRegenConfig())
		{
			AddResourceRegenEvent(resourceconfig.Value, resourceconfig.Key);
		}
	}
}

void UResourceGathererComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UResourceGathererComponent, Values);
	DOREPLIFETIME(UResourceGathererComponent, ResourceMaximums);
	DOREPLIFETIME(UResourceGathererComponent, MaxWeight);
}
