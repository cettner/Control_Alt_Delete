// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GenericTeamAgentInterface.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BrainComponent.h"

#include "../../Orders/RTSOrder.h"
#include "Navigation/FlockPathFollowingComponent.h"
#include "RTSAIPerceptionComponent.h"
#include "RTSAIController.generated.h"

USTRUCT()
struct FRTSAIPerceptionConfig
{
   GENERATED_USTRUCT_BODY()
public:
	UPROPERTY(EditDefaultsOnly)
	float SightRadius = 0.0;

	UPROPERTY(EditDefaultsOnly)
	float LoseSightRadius = 0.0;

	UPROPERTY(EditDefaultsOnly)
	float PeripheralVision = 0.0;

	UPROPERTY(EditDefaultsOnly)
	FAISenseAffiliationFilter SightAffiliation = FAISenseAffiliationFilter();
};

enum ERTSOrderFinishReason
{
	ORDERCOMPLETE,
	ORDERFAIL,
	ORDERINTERRUPTED
};


UCLASS()
class RTS_PROJECT_API ARTSAIController : public AAIController
{
	GENERATED_BODY()
	
public:

	ARTSAIController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void OnPossess(APawn *InPawn) override;
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

protected:
	virtual void PostInitializeComponents() override;

public:
	class ACommander * GetLeadRTSObject();
	void ClearCommander();
	void SetCommander(ACommander * Commander);

	FORCEINLINE FAIRequestID GetAIRequestId() const { return AIRequestId; }
	void SendAIMessage(const FName AIMessage, FAIMessage::EStatus Status);

public:
	virtual const URTSOrder* GetCurrentOrder() const;
	virtual void EnqueueOrder(const URTSOrder* InOrder, bool InbIsEnquedOrder = false);
	virtual bool IsOrderAvailable()  const;

protected:
	virtual void ClearOrders();
	virtual void SetCurrentOrder(const URTSOrder* InOrder);
	virtual void OnOrderFinished(const URTSOrder* InOrder, const ERTSOrderFinishReason InFinishReason);

protected:
	virtual void ActorsPerceptionUpdated(const TArray<AActor*>& UpdatedActors) override;

protected:
	UFUNCTION()
	virtual void OnTargetPerceptionUpdated(AActor * Actor, FAIStimulus Stimulus);

	virtual bool ConfigureRTSPerception(class ARTSMinion * Minion);


public:
	static const FName AIMessage_Finished;


protected:
	UPROPERTY(transient)
	class UBlackboardComponent* BlackboardComp;

	UPROPERTY(transient)
	class UBehaviorTreeComponent* BehaviorComp;

	UPROPERTY(transient)
	URTSAIPerceptionComponent* PerceptionComp;

	UPROPERTY(transient)
	UAISenseConfig_Sight* SightConfig;

	UPROPERTY(EditDefaultsOnly)
	FName PrimaryOrder = "PrimaryOrder";

protected:
	UFlockPathFollowingComponent* FlockPathingComp;


private:

	FRTSAIPerceptionConfig DefaultPerceptionConfig;

	FAIRequestID AIRequestId;
	FORCEINLINE void StoreAIRequestId() { AIRequestId = AIRequestId + 1; }


protected:
	const URTSOrder* CurrentOrder = nullptr;

	TQueue<const URTSOrder*> EnquedOrders;

	int NumOrders = 0;
};
