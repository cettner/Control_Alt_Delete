// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BTDecorator_IsTargetEnemy.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UBTDecorator_IsTargetEnemy : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()

    public:
       UBTDecorator_IsTargetEnemy();

    protected:
	   virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

};
