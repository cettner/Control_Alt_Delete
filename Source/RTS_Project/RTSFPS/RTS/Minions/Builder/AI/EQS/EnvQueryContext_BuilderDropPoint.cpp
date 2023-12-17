// Fill out your copyright notice in the Description page of Project Settings.


#include "EnvQueryContext_BuilderDropPoint.h"
#include "RTS_Project/RTSFPS/RTS/Minions/AI/RTSAIController.h"
#include "RTS_Project/RTSFPS/RTS/Orders/RTSMineResourceOrder.h"

#include "EnvironmentQuery/EnvQueryTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"

void UEnvQueryContext_BuilderDropPoint::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	APawn* QuerierActor = Cast<APawn>(QueryInstance.Owner.Get());
	if ((QuerierActor == nullptr))
	{
		return;
	}

	ARTSAIController * aic = QuerierActor->GetController<ARTSAIController>();
	if (aic == nullptr) return;

	AActor * droppointactor = Cast<AActor>(aic->GetBlackBoardKeyAsObject(URTSMineResourceOrder::ResourceDroppointKey));

	if (droppointactor)
	{
		UEnvQueryItemType_Actor::SetContextHelper(ContextData, droppointactor);
	}
}
