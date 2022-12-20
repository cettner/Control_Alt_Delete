// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_RTSBlackboardBase.h"
#include "../RTSAIController.h"



void UBTTask_RTSBlackboardBase::OnMessage(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, FName Message, int32 RequestID, bool bSuccess)
{
	if (Message == ARTSAIController::AIMessageAbortRequest)
	{
		FinishLatentAbort(OwnerComp);
	}
	else if (Message == ARTSAIController::AIMessageOrderRequest && bSuccess == true)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	else if (Message == ARTSAIController::AIMessageOrderRequest && bSuccess == false)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}
