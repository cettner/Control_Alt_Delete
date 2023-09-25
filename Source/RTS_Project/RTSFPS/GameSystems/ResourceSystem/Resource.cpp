// Fill out your copyright notice in the Description page of Project Settings.


#include "Resource.h"

bool UResource::CanAfford(FReplicationResourceMap BuyerResources, FReplicationResourceMap SellerCost)
{
	bool retval = true;
	const TMap<TSubclassOf<UResource>, int> buyermap = BuyerResources.GetMap();

	for (TPair<TSubclassOf<UResource>, int> Elem : SellerCost.GetMap())
	{
		const int32* outval = buyermap.Find(Elem.Key);
		if (outval != nullptr && (retval == true))
		{
			/*Buyer has the resource, now check they have enough*/
			retval &= (*outval >= Elem.Value);
		}
		else
		{
			/*Buyer did not have the needed Resource or did not have enough*/
			retval = false;
			break;
		}
	}

	return retval;
}

uint32 UResource::GetResourceWeight() const
{
	if (!IsWeightedResource())
	{
		UE_LOG(LogTemp, Warning, TEXT("Warning: GetResourceWeight was called on %s which is a discrete resource"), *GetName());
	}

	return ResourceWeight;
}

FName UResource::GetResourceName() const
{
	return ResourceName;
}
