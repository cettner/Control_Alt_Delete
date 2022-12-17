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
    EBTNodeResult::Type retval = EBTNodeResult::Failed;
    const ARTSAIController * Controller = Cast<ARTSAIController>(OwnerComp.GetAIOwner());
    AResource * target = Cast<AResource>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(ResourceNodeKey.SelectedKeyName));

    if(target)
    {
        ARTSBUILDER * minion = Cast<ARTSBUILDER>(Controller->GetPawn());
        if(!minion->IsMining())
        {   
            minion->StartMining(target);
            WaitForMessage(OwnerComp, ARTSAIController::AIMessageOrderRequest, Controller->GetAIRequestId());
            retval = EBTNodeResult::InProgress;
        }
    }

    return retval;
}

EBTNodeResult::Type UBTTask_Builder_Mine_Node::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    EBTNodeResult::Type retval = EBTNodeResult::Succeeded;
    const ARTSAIController* controller = Cast<ARTSAIController>(OwnerComp.GetAIOwner());
    ARTSBUILDER* builder = Cast<ARTSBUILDER>(controller->GetPawn());

    if (!builder->StopMining())
    {
        WaitForMessage(OwnerComp, ARTSAIController::AIMessageAbortRequest, controller->GetAIAbortId());
        retval = EBTNodeResult::InProgress;
    }

    return retval;
}


