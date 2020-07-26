// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_UpdateKeyPerception.h"
#include "RTS_Project/RTSFPS/RTS/Minions/AI/RTSAIController.h"
#include "RTS_Project/RTSFPS/BaseClasses/RTSMinion.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"



EBTNodeResult::Type UBTTask_UpdateKeyPerception::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ARTSAIController* Controller = Cast<ARTSAIController>(OwnerComp.GetAIOwner());

	TArray<AActor*> PercievedActors;
	Controller->GetPerceptionComponent()->GetCurrentlyPerceivedActors(UAISense_Sight::StaticClass(), PercievedActors);

	if (PercievedActors.Num() >0)
	{
		for (int i = 0; i < PercievedActors.Num(); i++)
		{
			if (!PercievedActors[i]->IsPendingKill())
			{
				OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), PercievedActors[0]);
			}
		}

		return(EBTNodeResult::Succeeded);
	}
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), nullptr);
	return(EBTNodeResult::Failed);
}