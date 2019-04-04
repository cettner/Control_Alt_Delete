// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BT_Can_Builder_Carry_More.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UBT_Can_Builder_Carry_More : public UBTDecorator
{
	GENERATED_BODY()
	
		virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
	
};
