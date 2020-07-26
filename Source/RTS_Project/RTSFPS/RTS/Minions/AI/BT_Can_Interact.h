// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_BlackboardBase.h"
#include "BT_Can_Interact.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UBT_Can_Interact : public UBTDecorator_BlackboardBase
{
	GENERATED_BODY()
   public: 
	   UBT_Can_Interact();

   protected:
	   virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
	
};
