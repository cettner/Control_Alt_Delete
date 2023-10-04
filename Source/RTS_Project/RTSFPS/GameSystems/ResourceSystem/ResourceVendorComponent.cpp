// Fill out your copyright notice in the Description page of Project Settings.


#include "ResourceVendorComponent.h"

#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UResourceVendorComponent::UResourceVendorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	StartingPrices = CreateDefaultSubobject<UResourceVendorData>(TEXT("Starting Prices"));
}

const TMap<TSubclassOf<UObject>, FReplicationResourceMap> UResourceVendorComponent::GetAllDefaultUnitPrices() const
{
	return StartingPrices->GetDefaultPriceMap();
}

bool UResourceVendorComponent::GetUnitPriceForSource(const TSubclassOf<UObject> PurchaseClass, const IResourceGatherer* Purchaser, FReplicationResourceMap& OutPrices, const AController* InstigatingController) const
{
	bool retval = false;
	const int32 * index = PriceIndexMap.Find(PurchaseClass);

	if (index != nullptr)
	{
		OutPrices = ReplicatedPrices[*index];
		retval = true;
	}

	return retval;
}

void UResourceVendorComponent::OnRegister()
{
	Super::OnRegister();

	int index = 0;
	for (TPair<TSubclassOf<UObject>, FReplicationResourceMap> PriceMap : StartingPrices->GetDefaultPriceMap())
	{
		PriceIndexMap.Emplace(PriceMap.Key, index);
		ReplicatedPrices.Emplace(PriceMap.Value);
		index++;
	}
}

void UResourceVendorComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UResourceVendorComponent, ReplicatedPrices);
}

