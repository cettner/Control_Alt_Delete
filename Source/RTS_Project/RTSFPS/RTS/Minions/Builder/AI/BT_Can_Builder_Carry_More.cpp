// Fill out your copyright notice in the Description page of Project Settings.

#include "BT_Can_Builder_Carry_More.h"
#include "RTS_Project/RTSFPS/RTS/Minions/AI/RTSAIController.h"
#include "RTS_Project/RTSFPS/RTS/Minions/Builder/RTSBUILDER.h"
#include "RTS_Project/RTSFPS/GameSystems/ResourceSystem/Resource.h"

#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"


bool UBT_Can_Builder_Carry_More::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool result = false;
    ARTSAIController * Controller = Cast<ARTSAIController>(OwnerComp.GetAIOwner());
    AResource * target = Cast<AResource>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>("Target"));

    if(Controller && target)
    {
        ARTSBUILDER * minion = Cast<ARTSBUILDER>(Controller->GetPawn());
        if(minion)
        {
			result = minion->CanCarryMore();
        }
    }

	return(result);
}