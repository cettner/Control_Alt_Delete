// Fill out your copyright notice in the Description page of Project Settings.
#include "BTTask_March_With_Commander.h"
#include "RTSAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "RTSMinion.h"





EBTNodeResult::Type UBTTask_March_With_Commander::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ARTSAIController * Controller = Cast<ARTSAIController>(OwnerComp.GetAIOwner());
	ARTSMinion * Minion = Cast<ARTSMinion>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>("Target"));

	
	if (Minion)
	{

		return(EBTNodeResult::Succeeded);
	}
	else
	{
		return (EBTNodeResult::Failed);
	}
}