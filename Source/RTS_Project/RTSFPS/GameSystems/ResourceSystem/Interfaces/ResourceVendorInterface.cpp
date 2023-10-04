// Fill out your copyright notice in the Description page of Project Settings.


#include "ResourceVendorInterface.h"

// Add default functionality here


bool IResourceVendorInterface::PurchaseUnit(const TSubclassOf<UObject> PurchaseClass, IResourceGatherer* Purchaser, AController* InstigatingController)
{
	bool retval = false;

	if (IsUnitPurchaseable(PurchaseClass, Purchaser, InstigatingController))
	{
		FReplicationResourceMap costs = FReplicationResourceMap();
		checkf(GetUnitPriceForSource(PurchaseClass, Purchaser, costs, InstigatingController), TEXT("IResourceVendorInterface::PurchaseUnit Failed to aquire Price Despite Being able to Purchase"));
		
		retval = Purchaser->RemoveResource(costs);
	}
	
	return retval;
}

const FReplicationResourceMap IResourceVendorInterface::GetDefaultUnitPrice(const TSubclassOf<UObject> PurchaseClass) const
{
	FReplicationResourceMap retval = FReplicationResourceMap();
	const TMap<TSubclassOf<UObject>, FReplicationResourceMap> catalogue = GetAllDefaultUnitPrices();
	const FReplicationResourceMap * priceptr = catalogue.Find(PurchaseClass);
	checkf(priceptr, TEXT("IResourceVendorInterface::GetDefaultUnitPrice Failed to find PurchaseClass"));

	return *priceptr;
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
	FReplicationResourceMap resourcetoreturn = FReplicationResourceMap();
	checkf(GetUnitPriceForSource(RefundClass, ToRefund, resourcetoreturn, InstigatingController), TEXT("IResourceVendorInterface::PurchaseUnit Failed to aquire Price Despite Being able to Purchase"));
	ToRefund->AddResource(resourcetoreturn);
	return true;
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

bool IResourceVendorInterface::GetUnitPriceForSource(const TSubclassOf<UObject> InPurchaseClass, const IResourceGatherer* Purchaser, FReplicationResourceMap& OutPrices, const AController* InstigatingController) const
{
	bool retval = false;
	if (IsUnitPurchaseable(InPurchaseClass, Purchaser, InstigatingController))
	{
		OutPrices = GetDefaultUnitPrice(InPurchaseClass);
		retval = true;
	}

	return retval;
}

TArray<TSubclassOf<UObject>> IResourceVendorInterface::GetPurchasableUnitsForSource(const IResourceGatherer* Purchaser , const AController* InstigatingController) const
{
	return GetAllPurchaseableUnits();
}

TMap<TSubclassOf<UObject>, FReplicationResourceMap> IResourceVendorInterface::GetAllUnitPricesForSource(const IResourceGatherer* Purchaser, const AController* InstigatingController) const
{
	return GetAllDefaultUnitPrices();
}
