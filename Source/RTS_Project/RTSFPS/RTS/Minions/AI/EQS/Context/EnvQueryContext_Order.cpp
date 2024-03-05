// Fill out your copyright notice in the Description page of Project Settings.


#include "EnvQueryContext_Order.h"
#include "EnvironmentQuery/EnvQueryTypes.h"

#include "RTS_Project/RTSFPS/RTS/Orders/RTSOrder.h"
#include "RTS_Project/RTSFPS/RTS/RTSMinion.h"
#include "EnvQueryItemType_Order.h"

void UEnvQueryContext_Order::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	if (const URTSOrder* order = Cast<URTSOrder>(QueryInstance.Owner.Get()))
	{
		UEnvQueryItemType_Order::SetContextHelper(ContextData, order);
	}
	else if (const ARTSMinion * minion = Cast<ARTSMinion>(QueryInstance.Owner.Get()))
	{
		UEnvQueryItemType_Order::SetContextHelper(ContextData, minion->GetCurrentOrder());
	}
}
