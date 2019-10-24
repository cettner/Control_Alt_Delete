// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_IsTargetEnemy.h"
#include "RTSAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "RTSMinion.h"


bool UBTDecorator_IsTargetEnemy::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool result = false;
	ARTSAIController* Controller = Cast<ARTSAIController>(OwnerComp.GetAIOwner());
	AActor* target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>("Target"));

	if (Controller && target)
	{
		ARTSMinion * Minion = Cast<ARTSMinion>(Controller->GetPawn());
		if (Minion)
		{
			result = Minion->IsEnemy(target);
		}
	}

	return (result);
}
