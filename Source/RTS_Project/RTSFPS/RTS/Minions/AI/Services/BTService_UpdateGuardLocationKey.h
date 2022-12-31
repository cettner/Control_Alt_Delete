// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_UpdateGuardLocationKey.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UBTService_UpdateGuardLocationKey : public UBTService_BlackboardBase
{
	GENERATED_BODY()
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
