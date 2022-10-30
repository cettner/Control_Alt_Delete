// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Interface.h"
#include "Containers/Map.h"

#include "../Resource.h"
#include "ResourcePurchasableInterface.h"
#include "ResourceGatherer.h"
#include "ResourceVendorInterface.generated.h"


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UResourceVendorInterface : public UInterface
{
	GENERATED_BODY()
};


class RTS_PROJECT_API IResourceVendorInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual bool PurchaseUnit(const TSubclassOf<UObject> PurchaseClass, IResourceGatherer * Purchaser, AController * InstigatingController = nullptr);
	virtual bool RefundUnit(const TSubclassOf<UObject> RefundClass, IResourceGatherer * ToRefund, AController* InstigatingController = nullptr);
	virtual bool IsUnitPurchaseable(const TSubclassOf<UObject> PurchaseClass, const IResourceGatherer * Purchaser, const AController* InstigatingController = nullptr) const;
	virtual FReplicationResourceMap GetUnitPriceForSource(const TSubclassOf<UObject> PurchaseClass, const IResourceGatherer* Purchaser, const AController* InstigatingController = nullptr) const;
	virtual TArray<TSubclassOf<UObject>> GetPurchasableUnitsForSource(const IResourceGatherer* Purchaser = nullptr, const AController* InstigatingController = nullptr) const;
	virtual TMap<TSubclassOf<UObject>, FReplicationResourceMap> GetAllUnitPricesForSource(const IResourceGatherer* Purchaser, const AController* InstigatingController = nullptr) const;
	
protected:
	virtual FReplicationResourceMap GetDefaultUnitPrice(const TSubclassOf<UObject> PurchaseClass) const;
	virtual TArray<TSubclassOf<UObject>> GetAllPurchaseableUnits() const;
	virtual TMap<TSubclassOf<UObject>, FReplicationResourceMap> GetAllDefaultUnitPrices() const = 0U;
};
