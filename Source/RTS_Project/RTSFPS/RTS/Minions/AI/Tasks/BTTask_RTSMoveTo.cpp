// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_RTSMoveTo.h"
#include "RTS_Project/RTSFPS/RTS/Minions/AI/RTSAIController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"


UBTTask_RTSMoveTo::UBTTask_RTSMoveTo()
{
	NodeName = "RTS Move To";
	/*Order may change, so notfy the node when the key has changed*/
	bObserveBlackboardValue = true;
}

EBTNodeResult::Type UBTTask_RTSMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type retval = Super::ExecuteTask(OwnerComp, NodeMemory);

	if (retval != EBTNodeResult::InProgress)
	{
		ARTSAIController* controller = Cast<ARTSAIController>(OwnerComp.GetAIOwner());
		bool success = false;
		if (retval == EBTNodeResult::Succeeded)
		{
			success = true;
		}

		controller->OnOrderFinished(this, success);
	}

	return retval;
}