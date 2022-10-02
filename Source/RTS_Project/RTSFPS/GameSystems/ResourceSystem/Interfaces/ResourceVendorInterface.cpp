// Fill out your copyright notice in the Description page of Project Settings.


#include "ResourceVendorInterface.h"

// Add default functionality here


bool IResourceVendorInterface::PurchaseUnit(const TSubclassOf<UObject> PurchaseClass, IResourceGatherer* Purchaser, AController* InstigatingController)
{
	return false;
}

FReplicationResourceMap IResourceVendorInterface::GetDefaultUnitPrice(const TSubclassOf<UObject> PurchaseClass) const
{
	return FReplicationResourceMap();
}

FReplicationResourceMap IResourceVendorInterface::RefundUnit(const TSubclassOf<UObject> RefundClass, IResourceGatherer* ToRefund, AController* InstigatingController)
{
	return FReplicationResourceMap();
}

bool IResourceVendorInterface::IsUnitPurchaseable(const TSubclassOf<UObject> PurchaseClass, IResourceGatherer* Purchaser, AController* InstigatingController) const
{
	return false;
}

FReplicationResourceMap IResourceVendorInterface::GetUnitPriceForSource(const TSubclassOf<UObject> PurchaseClass, IResourceGatherer* Purchaser, AController* InstigatingController) const
{
	return FReplicationResourceMap();
}

TArray<TSubclassOf<UObject>> IResourceVendorInterface::GetPurchasableUnits() const
{
	return TArray<TSubclassOf<UObject>>();
}

TMap<TSubclassOf<UObject>, FReplicationResourceMap> IResourceVendorInterface::GetAllDefaultUnitPrices() const
{
	return TMap<TSubclassOf<UObject>, FReplicationResourceMap>();
}

TMap<TSubclassOf<UObject>, FReplicationResourceMap> IResourceVendorInterface::GetAllUnitPricesForSource(IResourceGatherer* Purchaser, AController* InstigatingController) const
{
	return GetAllDefaultUnitPrices();
}
