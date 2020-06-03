// Fill out your copyright notice in the Description page of Project Settings.

#include "BTT_IsCommander.h"
#include "RTSAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "RTSFPS/FPS/Commander.h"

bool UBTT_IsCommander::CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const
{
	
	ACommander * commander = Cast<ACommander>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>("Target"));

	if (commander)
	{
		return true;
	}
	else
	{
		return false;
	}
	
}
