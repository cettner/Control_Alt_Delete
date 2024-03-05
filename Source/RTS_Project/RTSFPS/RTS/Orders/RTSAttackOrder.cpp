// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSAttackOrder.h"

void URTSAttackOrder::LoadAIBlackBoard(UBlackboardComponent* InBlackBoard) const
{
	const FOrderContext& ordercontext = GetOrderGroup()->GetOrderContext();
	InBlackBoard->SetValueAsObject("Target", Cast<AActor>(ordercontext.GetRTSContext()));
}
