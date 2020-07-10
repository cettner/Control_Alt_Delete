// Fill out your copyright notice in the Description page of Project Settings.

#include "BT_Can_Interact.h"
#include "RTS_Project/RTSFPS/RTS/Minions/AI/RTSAIController.h"
#include "RTS_Project/RTSFPS/BaseClasses/RTSMinion.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

bool UBT_Can_Interact::CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const
{
    ARTSAIController * Controller = Cast<ARTSAIController>(OwnerComp.GetAIOwner());
    AActor * target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>("Target"));

    if(Controller && target)
    {
        ARTSMinion * minion = Cast<ARTSMinion>(Controller->GetPawn());
        return(minion->CanInteract(target));
    }
    return(false);
}

