// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSOrderGroup.h"
#include "RTSTargetedOrder.h"
#include "RTSOrderManager.h"
#include "../../Shared/Interfaces/RTSObjectInterface.h"

TArray<TScriptInterface<IRTSObjectInterface>> URTSOrderGroup::GetAllActiveUnits() const
{
	TArray<TScriptInterface<IRTSObjectInterface>> retval = TArray<TScriptInterface<IRTSObjectInterface>>();

	return retval;
}

const TArray<URTSOrder *> URTSOrderGroup::GetOrders() const
{
	TArray<URTSOrder *> retval = TArray<URTSOrder*>();
	OrderMapping.GetKeys(retval);
	return retval;
}

bool URTSOrderGroup::InitalizeOrderGroup(const uint32 InID, const TArray<TScriptInterface<IRTSObjectInterface>>& InUnits, AController* Issuer, const FHitResult& InHitContext, const bool InbIsQueuedOrder)
{
	const int32 unitcount = InUnits.Num();
	int32 ordercount = 0;

	if (unitcount == 0) return false;

	OrderIssuer = Issuer;
	OrderID = InID;

	TMultiMap<TSubclassOf<URTSTargetedOrder>, TScriptInterface<IRTSObjectInterface>> ordermap = TMultiMap<TSubclassOf<URTSTargetedOrder>, TScriptInterface<IRTSObjectInterface>>();
	for (int i = 0; i < InUnits.Num(); i++)
	{
		if (IsValid(InUnits[i].GetObject()))
		{
			const TSubclassOf<URTSTargetedOrder> defaultorder = InUnits[i]->GetDefaultOrderClass(InHitContext);
			/*This can return null if the unit has nothing to do for the given context*/
			if (defaultorder != nullptr)
			{
				ordermap.Add(defaultorder, InUnits[i]);
			}
		}
	}

	/*For each class, create a new order and assign it to the units that implement it*/
	TArray<TSubclassOf<URTSTargetedOrder>> keys = TArray<TSubclassOf<URTSTargetedOrder>>();
	TArray<TScriptInterface<IRTSObjectInterface>> outunits = TArray<TScriptInterface<IRTSObjectInterface>>();
	ordermap.GetKeys(keys);
	for (int i = 0; i < keys.Num(); i++)
	{
		outunits.Reset();
		URTSOrder * ordertoissue = CreateTargetOrder(keys[i], InHitContext);
		ordermap.MultiFind(keys[i], outunits);
		ordercount += outunits.Num();
		for (int k = 0; k < outunits.Num(); k++)
		{
			OrderMapping.Emplace(ordertoissue, outunits[k]);
		}
	}

	const bool retval = unitcount == ordercount;
	return retval;
}

bool URTSOrderGroup::IssueOrder()
{
	if (bIsIssued == true) return false;

	for (const TTuple<URTSOrder *, TScriptInterface<IRTSObjectInterface>> orderpair : OrderMapping)
	{
		orderpair.Value->IssueOrder(OrderIssuer, OriginalContext, orderpair.Key);
	}

	bIsIssued = true;

	return bIsIssued;
}

bool URTSOrderGroup::DeregisterUnit(const TScriptInterface<IRTSObjectInterface>& InUnit, URTSOrder * InKey)
{
	const bool retval = (OrderMapping.RemoveSingle(InKey, InUnit) == 1);
	if (OrderMapping.Num() == 0)
	{
		ARTSOrderManager * manager = CastChecked<ARTSOrderManager>(GetOuter());
		manager->OnGroupEmptied(this);
	}

	return retval;
}

bool URTSOrderGroup::MatchingContext(const FHitResult& InHitContext, const AController* InController) const
{
	return false;
}

URTSTargetedOrder* URTSOrderGroup::CreateTargetOrder(const TSubclassOf<URTSTargetedOrder>& OrderClass, const FHitResult& InHitContext)
{
	URTSTargetedOrder* retval = NewObject<URTSTargetedOrder>(this, OrderClass);
	retval->AddToRoot();
	retval->SetTargetContext(InHitContext);
	retval->SetOrderGroup(this);
	return retval;
}

void URTSOrderGroup::BeginDestroy()
{
	const TArray<URTSOrder*> orders = GetOrders();
	for (int i = 0; i < orders.Num(); i++)
	{
		if (IsValid(orders[i]))
		{
			orders[i]->RemoveFromRoot();
			orders[i]->ConditionalBeginDestroy();
		}
	}
	Super::BeginDestroy();
}
