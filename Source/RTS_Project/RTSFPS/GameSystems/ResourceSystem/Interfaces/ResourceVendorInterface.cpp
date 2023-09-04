// Fill out your copyright notice in the Description page of Project Settings.


#include "ResourceVendorInterface.h"

// Add default functionality here


bool IResourceVendorInterface::PurchaseUnit(const TSubclassOf<UObject> PurchaseClass, IResourceGatherer* Purchaser, AController* InstigatingController)
{
	bool retval = false;
	
	if (IsUnitPurchaseable(PurchaseClass, Purchaser, InstigatingController))
	{
		const FReplicationResourceMap costs = GetUnitPriceForSource(PurchaseClass, Purchaser, InstigatingController);
		retval = Purchaser->RemoveResource(costs);
	}
	
	return retval;
}

FReplicationResourceMap IResourceVendorInterface::GetDefaultUnitPrice(const TSubclassOf<UObject> PurchaseClass) const
{
	FReplicationResourceMap retval = FReplicationResourceMap();
	const TMap<TSubclassOf<UObject>, FReplicationResourceMap> catalogue = GetAllDefaultUnitPrices();
	const FReplicationResourceMap * priceptr = catalogue.Find(PurchaseClass);
	
	if (priceptr != nullptr)
	{
		retval = *priceptr;
	}

	return retval;
}

TArray<TSubclassOf<UObject>> IResourceVendorInterface::GetAllPurchaseableUnits() const
{
	const TMap<TSubclassOf<UObject>, FReplicationResourceMap> catalogue = GetAllDefaultUnitPrices();
	TArray<TSubclassOf<UObject>> retval = TArray<TSubclassOf<UObject>>();
	catalogue.GetKeys(retval);
	return retval;
}

bool IResourceVendorInterface::RefundUnit(const TSubclassOf<UObject> RefundClass, IResourceGatherer* ToRefund, AController* InstigatingController)
{
	const FReplicationResourceMap resourcetoreturn = GetUnitPriceForSource(RefundClass, ToRefund, InstigatingController);
	const bool retval = ToRefund->CanCarryAllResources(resourcetoreturn);

	if (retval == true)
	{
		ToRefund->AddResource(resourcetoreturn);
	}

	return retval;
}

bool IResourceVendorInterface::IsUnitPurchaseable(const TSubclassOf<UObject> PurchaseClass, const IResourceGatherer* Purchaser, const AController* InstigatingController) const
{
	const TMap<TSubclassOf<UObject>, FReplicationResourceMap> catalogue = GetAllUnitPricesForSource(Purchaser, InstigatingController);
	const FReplicationResourceMap * resourcecost = catalogue.Find(PurchaseClass);
	bool retval = false;

	if (resourcecost != nullptr)
	{
		const FReplicationResourceMap availableresource = Purchaser->GetAllHeldResources();
		retval = UResource::CanAfford(availableresource, *resourcecost);
	}

	
	return retval;
}

FReplicationResourceMap IResourceVendorInterface::GetUnitPriceForSource(const TSubclassOf<UObject> PurchaseClass, const IResourceGatherer* Purchaser, const AController* InstigatingController) const
{
	return GetDefaultUnitPrice(PurchaseClass);
}

TArray<TSubclassOf<UObject>> IResourceVendorInterface::GetPurchasableUnitsForSource(const IResourceGatherer* Purchaser , const AController* InstigatingController) const
{
	return GetAllPurchaseableUnits();
}

TMap<TSubclassOf<UObject>, FReplicationResourceMap> IResourceVendorInterface::GetAllUnitPricesForSource(const IResourceGatherer* Purchaser, const AController* InstigatingController) const
{
	return GetAllDefaultUnitPrices();
}
