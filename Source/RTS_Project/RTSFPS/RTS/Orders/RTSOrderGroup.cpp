// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSOrderGroup.h"
#include "RTSTargetedOrder.h"
#include "RTSOrderManager.h"
#include "../../Shared/Interfaces/RTSObjectInterface.h"

#include "EnvironmentQuery/EnvQueryManager.h"

TArray<TScriptInterface<IRTSObjectInterface>> URTSOrderGroup::GetAllActiveUnits() const
{
	TArray<TScriptInterface<IRTSObjectInterface>> retval = TArray<TScriptInterface<IRTSObjectInterface>>();

	for (const URTSOrder* order : OrderMapping)
	{
		retval.Append(order->AssignedUnits);
	}

	return retval;
}

const TSet<URTSOrder *>& URTSOrderGroup::GetOrders() const
{
	return OrderMapping;
}

bool URTSOrderGroup::InitalizeOrderGroup(const uint32 InID, const TArray<TScriptInterface<IRTSObjectInterface>>& InUnits, AController* Issuer, const FHitResult& InHitContext, const bool InbIsQueuedOrder)
{
	const int32 unitcount = InUnits.Num();
	int32 ordercount = 0;
	OrderContext = ReadyOrderContext(InHitContext);

	if (unitcount == 0  || !OrderContext.IsValid()) return false;

	OrderIssuer = Issuer;
	OrderID = InID;

	TMultiMap<TSubclassOf<URTSTargetedOrder>, TScriptInterface<IRTSObjectInterface>> ordermap = TMultiMap<TSubclassOf<URTSTargetedOrder>, TScriptInterface<IRTSObjectInterface>>();
	for (int i = 0; i < InUnits.Num(); i++)
	{
		if (IsValid(InUnits[i].GetObject()))
		{
			const TSubclassOf<URTSTargetedOrder> defaultorder = InUnits[i]->GetDefaultOrderClass(OrderContext);
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
		URTSOrder * ordertoissue = CreateTargetOrder(keys[i]);
		ordermap.MultiFind(keys[i], outunits);
		ordertoissue->InitRegistration(outunits);
		ordercount += ordertoissue->GetUnitCount();
		OrderMapping.Emplace(ordertoissue);
	}

	const bool retval = unitcount == ordercount;
	return retval;
}

FOrderContext URTSOrderGroup::ReadyOrderContext(const FHitResult& InHitContext)
{
	FOrderContext retval = FOrderContext();
	retval.ContextActor = Cast<IRTSObjectInterface>(InHitContext.GetActor());
	retval.OriginalContextHitPoint = InHitContext.Location;
	retval.LastValidOrderPoint = InHitContext.Location;
	retval.bIsValid = InHitContext.bBlockingHit;

	return retval;
}

bool URTSOrderGroup::IssueAllOrders()
{
	if (bIsIssued == true) return false;

	for (URTSOrder * orderpair : OrderMapping)
	{
		orderpair->IssueOrder();
	}

	bIsIssued = true;

	return bIsIssued;
}

bool URTSOrderGroup::AreQueryResultsAvailable(const TObjectPtr<UEnvQuery>& InTemplate) const
{
	const bool retval = QueryMapping.Find(InTemplate) != nullptr;
	return retval;
}

FQueryFinishedSignature* URTSOrderGroup::IsQueryPending(const TObjectPtr<UEnvQuery>& InTemplate)
{
	FQueryFinishedSignature * retval = PendingQueries.Find(InTemplate);
	return retval;
}

FQueryFinishedSignature* URTSOrderGroup::RequestStartQuery(const TObjectPtr<UEnvQuery>& InTemplate, URTSOrder* RequestingOrder)
{
	FQueryFinishedSignature* retval = nullptr;
	if (FQueryFinishedSignature* pendingsignature = IsQueryPending(InTemplate))
	{
		retval = pendingsignature;
	}
	else
	{
		FEnvQueryRequest queryrequest = FEnvQueryRequest(InTemplate, RequestingOrder);
		FQueryFinishedSignature& outsignature = PendingQueries.Emplace(InTemplate, FQueryFinishedSignature());
		outsignature.BindUObject(RequestingOrder, &URTSOrder::OnOrderQueryComplete);

		if (queryrequest.Execute(EEnvQueryRunMode::AllMatching, outsignature) > INDEX_NONE)
		{
			retval = &outsignature;
		}
	}

	return retval;
}

const TSharedPtr<FEnvQueryResult> URTSOrderGroup::GetQueryResults(const TObjectPtr<UEnvQuery>& InTemplate) const
{
	TSharedPtr<FEnvQueryResult> retval = TSharedPtr<FEnvQueryResult>();

	if (const TSharedPtr<FEnvQueryResult>* foundptr = QueryMapping.Find(InTemplate))
	{
		retval = *foundptr;
	}

	return retval;
}

void URTSOrderGroup::AddOrderQueryResult(const TObjectPtr<UEnvQuery>& InQueryClass, const TSharedPtr<FEnvQueryResult>& InResult)
{
	if (FQueryFinishedSignature* querycompletedelegate = PendingQueries.Find(InQueryClass))
	{
		querycompletedelegate->Unbind();
		PendingQueries.Remove(InQueryClass);
	}

	QueryMapping.Emplace(InQueryClass, InResult);
}

URTSTargetedOrder* URTSOrderGroup::CreateTargetOrder(const TSubclassOf<URTSTargetedOrder>& OrderClass)
{
	URTSTargetedOrder* retval = NewObject<URTSTargetedOrder>(this, OrderClass);
	retval->InitializeOrder(this);
	retval->OrderAbandonedDelegate.AddUObject(this, &URTSOrderGroup::OnOrderAbandoned);
	return retval;
}

void URTSOrderGroup::OnOrderAbandoned(URTSOrder* AbandonedOrder)
{
	OrderMapping.Remove(AbandonedOrder);
	if (OrderMapping.Num() == 0)
	{
		ARTSOrderManager* manager = CastChecked<ARTSOrderManager>(GetOuter());
		manager->OnGroupEmptied(this);
	}
}

FVector FOrderContext::GetContextPoint() const
{
	FVector retval = FVector();
	if (IRTSObjectInterface* rtsobject = GetRTSContext())
	{
		retval = CastChecked<AActor>(rtsobject)->GetActorLocation();
	}
	else if (LastValidOrderPoint != FVector())
	{
		retval = LastValidOrderPoint;
	}
	else
	{
		retval = OriginalContextHitPoint;
	}
	return retval;
}
