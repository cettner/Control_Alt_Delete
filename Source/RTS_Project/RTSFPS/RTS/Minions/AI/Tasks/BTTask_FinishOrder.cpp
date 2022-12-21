// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FinishOrder.h"
#include "..\RTSAIController.h"


EBTNodeResult::Type UBTTask_FinishOrder::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ARTSAIController* controller = Cast<ARTSAIController>(OwnerComp.GetAIOwner());
	controller->OnOrderFinished(this, bWasSuccessful);
	return EBTNodeResult::Succeeded;
}
