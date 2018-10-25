// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_CheckForCommand.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UBTService_CheckForCommand : public UBTService
{
	GENERATED_BODY()
	
public:
	UBTService_CheckForCommand();
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
