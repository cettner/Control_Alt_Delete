// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Attack_Target.h"
#include "RTS_Project/RTSFPS/RTS/Minions/AI/RTSAIController.h"
#include "RTS_Project/RTSFPS/BaseClasses/RTSMinion.h"

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

	AActor* target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(GetSelectedBlackboardKey()));
	ARTSAIController* Controller = Cast<ARTSAIController>(OwnerComp.GetAIOwner());

	if (Controller && target && !MessageRecieved)
	{
		ARTSMinion* Minion = Cast<ARTSMinion>(Controller->GetPawn());
		if (Minion)
		{
			Minion->StartAttack(target);
			WaitForMessage(OwnerComp, ARTSAIController::AIMessage_Finished, Controller->GetAIRequestId());
			Result = EBTNodeResult::InProgress;
		}
	}
	else if(MessageRecieved)
	{
		Result = EBTNodeResult::Succeeded;
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		MessageRecieved = false;
	}

	return(Result);
}

void UBTTask_Attack_Target::OnMessage(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, FName Message, int32 RequestID, bool bSuccess)
{
	MessageRecieved = true;
	ExecuteTask(OwnerComp, NodeMemory);
}
