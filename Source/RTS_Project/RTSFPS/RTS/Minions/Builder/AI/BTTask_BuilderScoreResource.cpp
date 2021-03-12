// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_BuilderScoreResource.h"
#include "RTS_Project/RTSFPS/RTS/Structures/RTSStructure.h"
#include "RTS_Project/RTSFPS/RTS/Minions/Builder/RTSBUILDER.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "AIController.h"

EBTNodeResult::Type UBTTask_BuilderScoreResource::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type result = EBTNodeResult::Failed;
	AAIController* Controller = OwnerComp.GetAIOwner();
	ARTSStructure* target = Cast<ARTSStructure>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(GetSelectedBlackboardKey()));

	if (Controller && target)
	{
		ARTSBUILDER* minion = Cast<ARTSBUILDER>(Controller->GetPawn());
		if (minion && minion->DeliverResources(target))
		{
			result = EBTNodeResult::Succeeded;
		}
	}
	return result;
}
