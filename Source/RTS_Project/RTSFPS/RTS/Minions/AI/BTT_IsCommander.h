// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BTT_IsCommander.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UBTT_IsCommander : public UBTDecorator
{
	GENERATED_BODY()
		virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
};
