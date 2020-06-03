// Fill out your copyright notice in the Description page of Project Settings.

#include "BT_IsResource.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "RTSFPS/GameObjects/Resource.h"

bool UBT_IsResource::CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const
{
	AResource * resource = Cast<AResource>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>("Target"));

	if (resource)
	{
		return true;
	}
	else
	{
		return false;
	}
}


