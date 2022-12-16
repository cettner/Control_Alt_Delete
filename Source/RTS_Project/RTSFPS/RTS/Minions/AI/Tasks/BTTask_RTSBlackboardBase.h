// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_RTSBlackboardBase.generated.h"

UENUM()
enum EOrderTaskCompletion
{
	NEVER,
	ON_SUCCESS,
	ON_FAILURE,
	ALWAYS
};


UCLASS()
class RTS_PROJECT_API UBTTask_RTSBlackboardBase : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

protected:
		virtual void OnMessage(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, FName Message, int32 RequestID, bool bSuccess) override;

protected:
	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<EOrderTaskCompletion> OrderCompletionCondition = EOrderTaskCompletion::ON_SUCCESS;
};
