// Fill out your copyright notice in the Description page of Project Settings.


#include "ResourceVendorComponent.h"

#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
UResourceVendorComponent::UResourceVendorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	StartingPrices = CreateDefaultSubobject<UResourceVendorData>(TEXT("Starting Prices"));
}

TMap<TSubclassOf<UObject>, FReplicationResourceMap> UResourceVendorComponent::GetAllDefaultUnitPrices() const
{
	return TMap<TSubclassOf<UObject>, FReplicationResourceMap>();
}

FReplicationResourceMap UResourceVendorComponent::GetUnitPriceForSource(const TSubclassOf<UObject> PurchaseClass, const IResourceGatherer* Purchaser, const AController* InstigatingController) const
{
	return FReplicationResourceMap();
}

void UResourceVendorComponent::OnRegister()
{
	Super::OnRegister();
}

void UResourceVendorComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UResourceVendorComponent, ReplicatedPrices);
}

