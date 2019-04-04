// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTClearTarget.h"
#include "RTSAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Commander.h"



EBTNodeResult::Type UBTTClearTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    	ARTSAIController * Controller = Cast<ARTSAIController>(OwnerComp.GetAIOwner());
        ARTSMinion * minion = Cast<ARTSMinion>(Controller->GetPawn());

        if(Controller && minion)
        {
            if(minion->HasAssets())
            {   
                minion->ReleaseAssets();
            }
            minion->ClearTarget();
            return (EBTNodeResult::Succeeded);
        }
        else
        {
            return (EBTNodeResult::Failed);
        }
}