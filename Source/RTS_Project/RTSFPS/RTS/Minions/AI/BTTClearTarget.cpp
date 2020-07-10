// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTClearTarget.h"
#include "RTS_Project/RTSFPS/RTS/Minions/AI/RTSAIController.h"
#include "RTS_Project/RTSFPS/BaseClasses/RTSMinion.h"



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