// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSResourcePurchaseOrder.h"

#include "Net/UnrealNetwork.h"

void URTSResourcePurchaseOrder::SetPurchaseClass(const TSubclassOf<UObject> InClass)
{
	PurchaseClass = InClass;
}


void URTSResourcePurchaseOrder::SetPrice(const FReplicationResourceMap InPriceMap)
{
	PurchasePrice = InPriceMap;
}

TSubclassOf<UObject> URTSResourcePurchaseOrder::GetPurchaseClass() const
{
	return PurchaseClass;
}

FReplicationResourceMap URTSResourcePurchaseOrder::GetPurchasePrice() const
{
	return PurchasePrice;
}

URTSResourcePurchaseOrder::URTSResourcePurchaseOrder()
{
	bUseDefaultOnFail = false;
	PropertyApplicationCount = 1;
	bUseExactApplicationCountOnly = true;
}

void URTSResourcePurchaseOrder::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(URTSResourcePurchaseOrder, PurchaseClass, COND_InitialOnly);
	DOREPLIFETIME_CONDITION(URTSResourcePurchaseOrder, PurchasePrice, COND_InitialOnly);
}


