// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_IsTargetEnemy.h"
#include "RTS_Project/RTSFPS/RTS/RTSMinion.h"

#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"



UBTDecorator_IsTargetEnemy::UBTDecorator_IsTargetEnemy()
{
	NodeName = "Is Key Enemy";
}

bool UBTDecorator_IsTargetEnemy::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool result = false;
	AAIController* Controller = OwnerComp.GetAIOwner();
	AActor* target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(GetSelectedBlackboardKey()));

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
