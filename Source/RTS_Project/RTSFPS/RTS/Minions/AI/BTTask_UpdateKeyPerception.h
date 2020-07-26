// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_UpdateKeyPerception.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UBTTask_UpdateKeyPerception : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
    protected:
	    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
