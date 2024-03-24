// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSOrder.h"
#include "../../Shared/Interfaces/RTSObjectInterface.h"
#include "RTSOrderGroup.h"

#include "EnvironmentQuery/EnvQueryManager.h"

URTSOrder::URTSOrder() : Super()
{
}

TArray<TScriptInterface<IRTSObjectInterface>> URTSOrder::GetBestMinionsForOrder(const TArray<TScriptInterface<IRTSObjectInterface>>& InMinionSet, const FHitResult& InTarget) const
{
	const int applicationcount = GetApplicationCount();
	const bool canapplytocount = (InMinionSet.Num() >= applicationcount);
	TArray<TScriptInterface<IRTSObjectInterface>> retval = TArray<TScriptInterface<IRTSObjectInterface>>();


	/*Determine if the number of units ordered needs to be an exact amount or specific unit*/
	if (UsesExactApplicationCount() && canapplytocount)
	{
		for (int i = 0; i < applicationcount; i++)
		{
			retval.Emplace(InMinionSet[i]);
		}
	}
	else if (canapplytocount == true)
	{
		retval = TArray<TScriptInterface<IRTSObjectInterface>>(InMinionSet);
	}

	return retval;
}

URTSOrderGroup* URTSOrder::GetOrderGroup() const
{
	return OrderGroup;
}

uint32 URTSOrder::GetOrderID() const
{
	uint32 retval = INVALID_ORDER_ID;
	if (const URTSOrderGroup* ordergroup = GetOrderGroup())
	{
		retval = OrderGroup->GetOrderID();
	}
	return retval;
}

void URTSOrder::InitializeOrder(URTSOrderGroup* InOrderGroup)
{
	checkf(InOrderGroup, TEXT("URTSOrder::InitializeOrder OrderGroup was null"));
	OrderGroup = InOrderGroup;
	InitOrderContext(InOrderGroup->GetOrderContext());
	if (GetQueryForContext().Get() != nullptr)
	{
		StartOrderQuery();
	}
}

void URTSOrder::InitOrderContext(const FOrderContext& InContext)
{
}

void URTSOrder::InitRegistration(const TArray<TScriptInterface<IRTSObjectInterface>>& InUnits)
{
	AssignedUnits = TArray<TScriptInterface<IRTSObjectInterface>>(InUnits);
	URTSOrderGroup* ordergroup = GetOrderGroup();
	const FOrderContext& ordercontext = ordergroup->GetOrderContext();
	const FVector& contextpoint = ordercontext.GetContextPoint();

	// sort Assignedunits based on squared distance to the context point using IRTSObjectInterface::GetUnitLocation()
	AssignedUnits.Sort([contextpoint](const TScriptInterface<IRTSObjectInterface>& A, const TScriptInterface<IRTSObjectInterface>& B) {
		const FVector& LocationA = A->GetUnitLocation();
		const FVector& LocationB = B->GetUnitLocation();
		const float& DistanceSquaredA = FVector::DistSquared(contextpoint, LocationA);
		const float& DistanceSquaredB = FVector::DistSquared(contextpoint, LocationB);
		return DistanceSquaredA < DistanceSquaredB;
		});

}

bool URTSOrder::DeRegisterUnit(TScriptInterface<IRTSObjectInterface> InUnit)
{
	const int32 removednum = AssignedUnits.Remove(InUnit);
	if (AssignedUnits.Num() == 0)
	{
		OnOrderAbandoned();
	}
	const bool retval = removednum == 1;
	return retval;
}

void URTSOrder::OnOrderAbandoned()
{
	OrderAbandonedDelegate.Broadcast(this);
	OrderAbandonedDelegate.Clear();
}

bool URTSOrder::IssueOrder()
{
	URTSOrderGroup* ordergroup = GetOrderGroup();
	for (TScriptInterface<IRTSObjectInterface> unit : AssignedUnits)
	{
		unit->IssueOrder(ordergroup->GetOrderIssuer(), ordergroup->GetOrderContext(), this);
	}
	return true;
}

bool URTSOrder::StartOrderQuery()
{
	ActiveQuery = GetQueryForContext();
	PendingQueryDelegate = GetOrderGroup()->RequestStartQuery(ActiveQuery, this);

	return PendingQueryDelegate != nullptr;
}

bool URTSOrder::IsQueryResultAvailable() const
{
	return false;
}

void URTSOrder::OnOrderQueryComplete(TSharedPtr<FEnvQueryResult> InResult)
{
	checkf(ActiveQuery, TEXT("URTSOrder::OnOrderQueryComplete, NoActiveQueryFound"))

	if (URTSOrderGroup* ordergroup = GetOrderGroup())
	{
		ordergroup->AddOrderQueryResult(GetActiveQuery(), InResult);
	}

	ActiveQuery = nullptr;
	UpdateUnitsWithResults(InResult);
}

void URTSOrder::UpdateUnitsWithResults(TSharedPtr<FEnvQueryResult>& InResult)
{

}

TObjectPtr<UEnvQuery> URTSOrder::GetActiveQuery() const
{
	return ActiveQuery;
}

TObjectPtr<UEnvQuery> URTSOrder::GetQueryForContext() const
{
	return OrderQuery;
}

void URTSOrder::LoadAIBlackBoard(UBlackboardComponent* InBlackBoard) const
{

}

void URTSOrder::DescribeSelfToGameplayDebugger(FGameplayDebuggerCategory_Order * Debugger) const
{
	const URTSOrderGroup * ordergroup = GetOrderGroup();
	Debugger->StringPropertyMap.Emplace(TEXT("Specific OrderName "),  GetPropertyName().ToString());
	Debugger->IntegerPropertyMap.Emplace(TEXT("OrderID: "), GetOrderID());
	Debugger->IntegerPropertyMap.Emplace(TEXT("Order Count"), AssignedUnits.Num());
}