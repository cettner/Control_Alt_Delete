// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_Builder_Mine_Node.h"
#include "..\..\AI\RTSAIController.h"
#include "RTS_Project/RTSFPS/RTS/Minions/Builder/RTSBUILDER.h"
#include "RTS_Project/RTSFPS/GameSystems/ResourceSystem/Resource.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"


EBTNodeResult::Type UBTTask_Builder_Mine_Node::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    const ARTSAIController * Controller = Cast<ARTSAIController>(OwnerComp.GetAIOwner());
    AResource * target = Cast<AResource>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(ResourceNodeKey.SelectedKeyName));

    if(target)
    {
        ARTSBUILDER * minion = Cast<ARTSBUILDER>(Controller->GetPawn());
        if(minion)
        {
            if(!minion->IsMining())
            {   
                minion->StartMining(target);
                WaitForMessage(OwnerComp, ARTSBUILDER::AIMessageMineRequest, Controller->GetAIRequestId());
                return(EBTNodeResult::InProgress);
            }
            else // we have aquired all we can, 
            {
                FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
                return(EBTNodeResult::Succeeded);
            }               
        }
        else
        {
            FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
            return (EBTNodeResult::Failed);
        }
    }
    else
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
        return (EBTNodeResult::Failed);
    }
}

void UBTTask_Builder_Mine_Node::OnMessage(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, FName Message, int32 RequestID, bool bSuccess)
{
	ExecuteTask(OwnerComp, NodeMemory);
}

EBTNodeResult::Type UBTTask_Builder_Mine_Node::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    const ARTSAIController* controller = Cast<ARTSAIController>(OwnerComp.GetAIOwner());
    ARTSBUILDER* minion = Cast<ARTSBUILDER>(controller->GetPawn());

    return Super::AbortTask(OwnerComp, NodeMemory);
}


