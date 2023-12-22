// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSMoveOrder.h"

void URTSMoveOrder::SetTargetContext(const FHitResult& InContext)
{
	MoveToLocation = InContext.Location;
}

void URTSMoveOrder::LoadAIBlackBoard(UBlackboardComponent* InBlackBoard) const
{
	InBlackBoard->SetValueAsVector("MoveLocation", MoveToLocation);
}
