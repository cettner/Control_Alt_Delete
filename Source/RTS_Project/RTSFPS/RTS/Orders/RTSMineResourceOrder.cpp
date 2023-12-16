// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSMineResourceOrder.h"

const FName URTSMineResourceOrder::ResourceNodeKey = "TargetNode";
const FName URTSMineResourceOrder::ResourceClassKey = "ResourceClass";
const FName URTSMineResourceOrder::LastMinedLocationKey = "LastMinedLocation";
const FName URTSMineResourceOrder::ResourceDroppointKey = "ResourceDroppoint";

void URTSMineResourceOrder::SetTargetContext(AController* Issuer, const FHitResult& InContext)
{
	AMineableResource* node = CastChecked<AMineableResource>(InContext.GetActor());
	StartNode = TWeakObjectPtr<AMineableResource>(node);
	StartNodeLocation = node->GetActorLocation();
	ResourceClassToMine = node->GetResourceClass();
}

void URTSMineResourceOrder::LoadAIBlackBoard(UBlackboardComponent* InBlackBoard) const
{
	InBlackBoard->SetValueAsObject(ResourceNodeKey, StartNode.Get()); 
	InBlackBoard->SetValueAsObject(ResourceClassKey, ResourceClassToMine);
	InBlackBoard->SetValueAsVector(LastMinedLocationKey, StartNodeLocation);
}
