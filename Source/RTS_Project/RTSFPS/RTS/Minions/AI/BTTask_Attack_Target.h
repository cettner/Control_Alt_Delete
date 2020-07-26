// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_Attack_Target.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UBTTask_Attack_Target : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_Attack_Target();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void OnMessage(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, FName Message, int32 RequestID, bool bSuccess);

protected:
	bool MessageRecieved = false;
	EBTNodeResult::Type MessageResult;

};
