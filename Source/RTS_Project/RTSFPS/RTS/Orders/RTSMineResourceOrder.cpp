// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSMineResourceOrder.h"

const FName URTSMineResourceOrder::ResourceNodeKey = "TargetNode";
const FName URTSMineResourceOrder::ResourceClassKey = "ResourceClass";
const FName URTSMineResourceOrder::LastMinedLocationKey = "LastMinedLocation";
const FName URTSMineResourceOrder::ResourceDroppointKey = "ResourceDroppoint";

void URTSMineResourceOrder::LoadAIBlackBoard(UBlackboardComponent* InBlackBoard) const
{
	const FOrderContext& ordercontext = GetOrderGroup()->GetOrderContext();
	AMineableResource* node = Cast<AMineableResource>(ordercontext.GetRTSContext());

	InBlackBoard->SetValueAsObject(ResourceNodeKey, node);
	if (node)
	{
		InBlackBoard->SetValueAsObject(ResourceClassKey, node->GetResourceClass());
		InBlackBoard->SetValueAsVector(LastMinedLocationKey, ordercontext.GetContextPoint());
	}

}
