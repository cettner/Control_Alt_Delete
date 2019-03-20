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
	ACommander * target = Cast<ACommander>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>("Target"));

	if (target && Controller)
	{
		ACommander * Commander = Cast<ACommander>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>("OwningCommander"));

		/*Same commander, just move to it*/
		if (Commander == target)
		{
			ARTSMinion * minion = Cast<ARTSMinion>(Controller->GetPawn());
			FVector marchposition = target->GetMarchingOrder(minion);
			if (marchposition != FVector())
			{
				Controller->MoveToLocation(marchposition, 5.0f, false, true, true, true, 0, false);
			}
			
		}
		/*New Commander*/
		else if (target)
		{
				ARTSMinion * minion = Cast<ARTSMinion>(Controller->GetPawn());

				if (minion)
				{
					target->AddtoSquad(minion);
					OwnerComp.GetBlackboardComponent()->SetValueAsObject("OwningCommander", target);
					FVector position = target->GetActorLocation() + FVector(300, 300, 0);
					Controller->MoveToLocation(position, 5.0f, false, true, true, true, 0, false);
				}
		}
		else
		{

		}
		return(EBTNodeResult::Succeeded);
	}
	else
	{
		return (EBTNodeResult::Failed);
	}
}