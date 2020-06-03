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

	virtual void OnPossess(APawn *InPawn) override;

	void SetTarget(AActor* newtarget);
	AActor * GetTarget();
	void ClearTarget();

	class ACommander * GetCommander();
	void ClearCommander();
	void SetCommander(ACommander * Commander);
	FORCEINLINE FAIRequestID GetAIRequestId() const { return AIRequestId; }
	void SendAIMessage(const FName AIMessage, FAIMessage::EStatus Status);

public:
	static const FName AIMessage_Finished;



protected:
	UPROPERTY(EditDefaultsOnly)
	FName Target = "Target";

private:

	//static uint32 NextRequestId;
	FAIRequestID AIRequestId;
	FORCEINLINE void StoreAIRequestId() { AIRequestId = AIRequestId + 1; }

};
