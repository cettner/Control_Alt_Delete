// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "RTSAIController.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API ARTSAIController : public AAIController
{
	GENERATED_BODY()

		UPROPERTY(transient)
		class UBlackboardComponent *BlackboardComp;

	    UPROPERTY(transient)
		class UBehaviorTreeComponent *BehaviorComp;
	
public:

	ARTSAIController();

	virtual void Possess(APawn *InPawn) override;

	void SetTarget(AActor* newtarget);
	class ACommander * GetCommander();

	UPROPERTY(EditDefaultsOnly)
	FName Target = "Target";
};
