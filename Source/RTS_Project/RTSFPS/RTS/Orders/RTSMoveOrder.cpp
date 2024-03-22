// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSMoveOrder.h"
#include "RTSOrderGroup.h"

URTSMoveOrder::URTSMoveOrder() : Super()
{
	PropertyName = TEXT("Move Unit");
	PropertyDescription = FText::FromString("Move the Selected Units to The Target Position");
}

void URTSMoveOrder::LoadAIBlackBoard(UBlackboardComponent* InBlackBoard) const
{
	const FVector& movelocation = GetOrderGroup()->GetOrderContext().GetContextPoint();
	InBlackBoard->SetValueAsVector(TEXT("MoveLocation"), movelocation);
}
