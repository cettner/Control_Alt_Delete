// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_StoreKeyLocation.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UBTTask_StoreKeyLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	FName GetSelectedLocationKey() const;

protected:
	/** blackboard key selector */
	UPROPERTY(EditAnywhere, Category = Blackboard)
	struct FBlackboardKeySelector LocationKey;

};

FORCEINLINE FName UBTTask_StoreKeyLocation::GetSelectedLocationKey() const
{
	return LocationKey.SelectedKeyName;
}