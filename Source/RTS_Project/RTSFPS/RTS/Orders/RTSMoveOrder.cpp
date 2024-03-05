// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSMoveOrder.h"
#include "RTSOrderGroup.h"

void URTSMoveOrder::LoadAIBlackBoard(UBlackboardComponent* InBlackBoard) const
{
	const FVector& movelocation = GetOrderGroup()->GetOrderContext().GetContextPoint();
	InBlackBoard->SetValueAsVector("MoveLocation", movelocation);
}
