// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"

#include "BTTask_RTSMoveTo.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UBTTask_RTSMoveTo : public UBTTask_MoveTo
{
	GENERATED_BODY()

public:
	UBTTask_RTSMoveTo();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	bool MessageRecieved = false;
	EBTNodeResult::Type MessageResult;

};
