// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_IsOrderAvailable.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UBTDecorator_IsOrderAvailable : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
    public:
        UBTDecorator_IsOrderAvailable();

    protected:
		virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
        virtual EBlackboardNotificationResult OnBlackboardKeyValueChange(const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID) override;
};
