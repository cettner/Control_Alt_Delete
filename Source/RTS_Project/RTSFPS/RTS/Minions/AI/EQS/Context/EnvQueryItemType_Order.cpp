// Fill out your copyright notice in the Description page of Project Settings.


#include "EnvQueryItemType_Order.h"
#include "RTS_Project/RTSFPS/RTS/Orders/RTSOrderGroup.h"

#include "AITypes.h"

void UEnvQueryItemType_Order::SetContextHelper(FEnvQueryContextData& ContextData, const URTSOrder* SingleOrder)
{
	ContextData.ValueType = UEnvQueryItemType_Order::StaticClass();
	ContextData.NumValues = 1;
	ContextData.RawData.SetNumUninitialized(sizeof(FWeakObjectPtr));

	UEnvQueryItemType_Order::SetValue(ContextData.RawData.GetData(), SingleOrder);
}

URTSOrder* UEnvQueryItemType_Order::GetValue(const uint8* RawData)
{
	FWeakObjectPtr WeakObjPtr = GetValueFromMemory<FWeakObjectPtr>(RawData);
	return (URTSOrder*)(WeakObjPtr.Get());
}

void UEnvQueryItemType_Order::SetValue(uint8* RawData, const FWeakObjectPtr& Value)
{
	FWeakObjectPtr WeakObjPtr(Value);
	SetValueInMemory<FWeakObjectPtr>(RawData, WeakObjPtr);
}

FVector UEnvQueryItemType_Order::GetItemLocation(const uint8* RawData) const
{
	FVector retval = FAISystem::InvalidLocation;
	const URTSOrder* myorder = UEnvQueryItemType_Order::GetValue(RawData);
	if (myorder)
	{
		if (const URTSOrderGroup* ordergroup = myorder->GetOrderGroup())
		{
			const FOrderContext& ordercontext = ordergroup->GetOrderContext();
			retval = ordercontext.GetContextPoint();
		}
	}
	return retval;
}

URTSOrder* UEnvQueryItemType_Order::GetOrder(const uint8* RawData) const
{
	return UEnvQueryItemType_Order::GetValue(RawData);
}
