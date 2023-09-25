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

FOnResourceDepletedDelegate& UResourceGathererComponent::BindResourceDepletionEvent(TSubclassOf<UResource> InResourceType)
{
	if (FOnResourceDepletedDelegate* founddelegate = ResourceDelegates.Find(InResourceType))
	{
		return *founddelegate;
	}
	else
	{
		FOnResourceDepletedDelegate newdelegate = FOnResourceDepletedDelegate();
		ResourceDelegates.Emplace(InResourceType, newdelegate);

		return *ResourceDelegates.Find(InResourceType);
	}
}

void UResourceGathererComponent::AddResource(TSubclassOf<UResource> ResourceClass, int amount)
{
	const UResource* resourcecdo = ResourceClass.GetDefaultObject();
	checkf(HeldResources.Increment(ResourceClass, amount), TEXT("UResourceGathererComponent::AddResource, Failed to add Resource because it wasnt supported"));
	CurrentWeight += resourcecdo->GetResourceWeight() * amount;
}

bool UResourceGathererComponent::RemoveResource(const TSubclassOf<UResource> ResourceClass, int amount)
{
	const UResource* resourcecdo = ResourceClass.GetDefaultObject();
	bool retval = HeldResources.Decrement(ResourceClass, amount);
	CurrentWeight -= resourcecdo->GetResourceWeight() * amount;

	if (const int * outvalue = HeldResources.Find(ResourceClass))
	{
		if (*outvalue == 0)
		{
			FOnResourceDepletedDelegate* broadcast = ResourceDelegates.Find(ResourceClass);

			if (broadcast != nullptr)
			{
				(*broadcast).Broadcast(ResourceClass, this);
			}
		}
	}

	return retval;
}

FReplicationResourceMap UResourceGathererComponent::GetAllHeldResources() const
{
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

uint32 UResourceGathererComponent::GetResourceDiscreteMaximum(const TSubclassOf<UResource> ResourceClass) const
{

	return uint32();
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
		CurrentWeight += resourcecdo->GetResourceWeight() * HeldResources[i].Value;
	}
}

void UResourceGathererComponent::OnRep_HeldResources()
{
	RecalculateWeight();
}

void UResourceGathererComponent::OnRegister()
{
	Super::OnRegister();
	HeldResources = StartingResources->GetResourceData();
}

void UResourceGathererComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UResourceGathererComponent, HeldResources);
}
