// Fill out your copyright notice in the Description page of Project Settings.
#include "BTTask_March_With_Commander.h"
#include "RTSAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Commander.h"




EBTNodeResult::Type UBTTask_March_With_Commander::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ARTSAIController * Controller = Cast<ARTSAIController>(OwnerComp.GetAIOwner());
	ACommander * Commander = Cast<ACommander>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>("OwningCommander"));

	if (Controller && Commander)
	{
			ARTSMinion * minion = Cast<ARTSMinion>(Controller->GetPawn());
			FVector marchposition;
			if(Commander->GetMarchingOrder(minion, marchposition))
			{
				Controller->MoveToLocation(marchposition, 5.0f, false, true, true, true, 0, false);
				return(EBTNodeResult::Succeeded);
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