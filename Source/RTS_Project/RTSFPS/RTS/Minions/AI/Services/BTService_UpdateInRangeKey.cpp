// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_UpdateInRangeKey.h"
#include "..\..\..\RTSMinion.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"





UBTService_UpdateInRangeKey::UBTService_UpdateInRangeKey()
{
}

void UBTService_UpdateInRangeKey::TickNode(UBehaviorTreeComponent& OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	const AAIController * aic = OwnerComp.GetAIOwner();
	UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();
	const ARTSMinion* minion = aic->GetPawn<ARTSMinion>();
	const AActor* target = Cast<AActor>(blackboard->GetValue<UBlackboardKeyType_Object>(GetSelectedBlackboardKey()));
	
	const int32 attackindex = minion->GetAttackIndexForTarget(target);

	if (attackindex > CANT_ATTACK_INDEX)
	{
		blackboard->SetValueAsInt("AttackID", attackindex);
	}
	else
	{
		blackboard->SetValueAsInt("AttackID", CANT_ATTACK_INDEX);
	}



}
