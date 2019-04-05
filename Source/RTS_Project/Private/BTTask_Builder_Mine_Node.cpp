// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_Builder_Mine_Node.h"
#include "RTSAIController.h"
#include "RTSBUILDER.h"
#include "Resource.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"



EBTNodeResult::Type UBTTask_Builder_Mine_Node::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    ARTSAIController * Controller = Cast<ARTSAIController>(OwnerComp.GetAIOwner());
    AResource * target = Cast<AResource>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>("Target"));

    if(Controller && target)
    {
        ARTSBUILDER * minion = Cast<ARTSBUILDER>(Controller->GetPawn());
        if(minion)
        {
            bool can_carry = minion->CanCarryMore();
            if(can_carry && minion->CanMine())  //if we have room to carry and builder mine cooldown is ready
            {   
                minion->Mine_Resource(target);
                if(minion->CanCarryMore()) //check again cause we might have just filled up
                {
                    return(EBTNodeResult::InProgress);
                }
                else 
                {
                    return(EBTNodeResult::Succeeded);
                }
            }
            else if(can_carry) //we can carry more, but mine status isnt off cooldown
            {
                return(EBTNodeResult::InProgress);
            }
            else // we have aquired all we can, 
            {
                return(EBTNodeResult::Succeeded);
            }               
        }
        else
        {
            return (EBTNodeResult::Failed);
        }
    }
    else
    {
        return (EBTNodeResult::Failed);
    }
}