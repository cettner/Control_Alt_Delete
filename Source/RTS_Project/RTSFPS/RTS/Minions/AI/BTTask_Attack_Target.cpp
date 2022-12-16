// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack_Target.h"
#include "RTS_Project/RTSFPS/RTS/Minions/AI/RTSAIController.h"
#include "RTS_Project/RTSFPS/RTS/RTSMinion.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"


UBTTask_Attack_Target::UBTTask_Attack_Target()
{
	NodeName = "Attack BlackBoard Key";
}

EBTNodeResult::Type UBTTask_Attack_Target::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = EBTNodeResult::Failed;

	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();
	AActor* target = Cast<AActor>(blackboard->GetValue<UBlackboardKeyType_Object>(GetSelectedBlackboardKey()));
	ARTSAIController* Controller = Cast<ARTSAIController>(OwnerComp.GetAIOwner());

	if (target)
	{
		ARTSMinion* Minion = Cast<ARTSMinion>(Controller->GetPawn());
		const int32 attackid = blackboard->GetValueAsInt(GetSelectedBlackboardKey());
		if (Minion->StartAttack(attackid))
		{
			WaitForMessage(OwnerComp, ARTSAIController::AIMessageOrderRequest, Controller->GetAIRequestId());
			Result = EBTNodeResult::InProgress;
		}
	}

	return(Result);
}

EBTNodeResult::Type UBTTask_Attack_Target::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const ARTSAIController* controller = Cast<ARTSAIController>(OwnerComp.GetAIOwner());
	ARTSMinion* Minion = Cast<ARTSMinion>(controller->GetPawn());
	EBTNodeResult::Type retval = EBTNodeResult::Succeeded;

	if (!Minion->StopAttack())
	{
		WaitForMessage(OwnerComp, ARTSAIController::AIMessageAbortRequest, controller->GetAIAbortId());
		retval = EBTNodeResult::InProgress;
	}

	return retval;
}
