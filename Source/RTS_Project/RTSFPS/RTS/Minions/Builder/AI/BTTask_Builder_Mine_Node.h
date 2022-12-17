// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../AI/Tasks/BTTask_RTSBlackboardBase.h"
#include "BTTask_Builder_Mine_Node.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UBTTask_Builder_Mine_Node : public UBTTask_RTSBlackboardBase
{
	GENERATED_BODY()

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;	
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	protected:
		UPROPERTY(EditAnywhere, Category = Blackboard)
		FBlackboardKeySelector ResourceNodeKey;
};
