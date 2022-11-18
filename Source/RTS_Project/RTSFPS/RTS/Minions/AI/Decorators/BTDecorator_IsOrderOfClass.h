// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"

#include "..\..\..\Orders\RTSOrder.h"
#include "BTDecorator_IsOrderOfClass.generated.h"



/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UBTDecorator_IsOrderOfClass : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
    public:
        UBTDecorator_IsOrderOfClass();

    protected:
		virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

    protected:
        UPROPERTY(EditAnywhere)
        TSubclassOf<URTSOrder> OrderCheckClass = URTSOrder::StaticClass();
};
