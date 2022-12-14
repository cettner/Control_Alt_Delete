// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSMineResourceOrder.h"

void URTSMineResourceOrder::SetTargetContext(AController* Issuer, const FHitResult& InContext)
{
	AResource* node = CastChecked<AResource>(InContext.GetActor());
	StartNode = TWeakObjectPtr<AResource>(node);
	StartNodeLocation = node->GetActorLocation();
	ResourceClassToMine = node->GetClass();
}

void URTSMineResourceOrder::LoadAIBlackBoard(UBlackboardComponent* InBlackBoard) const
{
	InBlackBoard->SetValueAsObject("TargetNode", StartNode.Get()); 
	InBlackBoard->SetValueAsObject("ResourceClass", ResourceClassToMine);
	InBlackBoard->SetValueAsVector("LastMinedLocation", StartNodeLocation);
}
