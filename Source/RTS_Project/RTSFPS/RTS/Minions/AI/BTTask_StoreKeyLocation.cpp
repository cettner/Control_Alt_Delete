// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_StoreKeyLocation.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

EBTNodeResult::Type UBTTask_StoreKeyLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AActor* target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(GetSelectedBlackboardKey()));
	
	if (target)
	{
		FVector LocationtoStore = target->GetActorLocation();
		OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>(GetSelectedLocationKey(),LocationtoStore);
	}


	return(EBTNodeResult::Succeeded);
}