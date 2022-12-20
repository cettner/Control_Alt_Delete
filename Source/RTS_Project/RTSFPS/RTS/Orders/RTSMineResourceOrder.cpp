// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSMineResourceOrder.h"

const FName URTSMineResourceOrder::ResourceNodeKey = "TargetNode";
const FName URTSMineResourceOrder::ResourceClassKey = "ResourceClass";
const FName URTSMineResourceOrder::LastMinedLocationKey = "LastMinedLocation";

void URTSMineResourceOrder::SetTargetContext(AController* Issuer, const FHitResult& InContext)
{
	AResource* node = CastChecked<AResource>(InContext.GetActor());
	StartNode = TWeakObjectPtr<AResource>(node);
	StartNodeLocation = node->GetActorLocation();
	ResourceClassToMine = node->GetClass();
}

void URTSMineResourceOrder::LoadAIBlackBoard(UBlackboardComponent* InBlackBoard) const
{
	InBlackBoard->SetValueAsObject(ResourceNodeKey, StartNode.Get()); 
	InBlackBoard->SetValueAsObject(ResourceClassKey, ResourceClassToMine);
	InBlackBoard->SetValueAsVector(LastMinedLocationKey, StartNodeLocation);
}
