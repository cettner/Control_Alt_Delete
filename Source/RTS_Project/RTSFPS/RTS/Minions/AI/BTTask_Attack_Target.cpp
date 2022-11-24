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

	if (target && !MessageRecieved)
	{
		ARTSMinion* Minion = Cast<ARTSMinion>(Controller->GetPawn());
		const int32 attackid = blackboard->GetValueAsInt(GetSelectedBlackboardKey());
		if (Minion->StartAttack(attackid))
		{
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
