// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../AI/Tasks/BTTask_RTSBlackboardBase.h"
#include "BTTask_Attack_Target.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UBTTask_Attack_Target : public UBTTask_RTSBlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_Attack_Target();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
