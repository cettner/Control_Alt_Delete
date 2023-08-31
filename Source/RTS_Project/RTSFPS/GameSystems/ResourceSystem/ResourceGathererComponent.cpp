// Fill out your copyright notice in the Description page of Project Settings.


#include "ResourceGathererComponent.h"

// Sets default values for this component's properties
UResourceGathererComponent::UResourceGathererComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

}

FOnResourceDepletedDelegate& UResourceGathererComponent::BindResourceDepletionEvent(TSubclassOf<AResource> InResourceType)
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

void UResourceGathererComponent::AddResource(TSubclassOf<AResource> ResourceClass, int amount)
{
	Resources.Increment(ResourceClass, amount);
}

bool UResourceGathererComponent::RemoveResource(const TSubclassOf<AResource> ResourceClass, int amount)
{
	bool retval = Resources.Decrement(ResourceClass, amount);
	
	if (const int * outvalue = Resources.Find(ResourceClass))
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

void UResourceGathererComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}
