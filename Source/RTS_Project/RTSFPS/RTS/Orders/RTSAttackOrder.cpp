// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSAttackOrder.h"

void URTSAttackOrder::SetTargetContext(AController* Issuer, const FHitResult& InContext)
{
	Target = InContext.GetActor();
}

void URTSAttackOrder::LoadAIBlackBoard(UBlackboardComponent* InBlackBoard) const
{
	InBlackBoard->SetValueAsObject("Target", Target.GetObject());
}
