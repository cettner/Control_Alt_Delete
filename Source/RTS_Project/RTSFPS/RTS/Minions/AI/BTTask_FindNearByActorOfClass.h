// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_FindNearByActorOfClass.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UBTTask_FindNearByActorOfClass : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category = BlackBoard)
	TSubclassOf<AActor> SearchClass = AActor::StaticClass();

	UPROPERTY(EditAnywhere, Category = BlackBoard)
	float SearchDistance = 1000.0f;

	UPROPERTY(EditAnywhere, Category = BlackBoard)
	bool bfindClosest = false;

};
