// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GenericTeamAgentInterface.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BrainComponent.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"

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

enum EAIMessageType
{
	Task,
	Abort,
	Progress
};

UCLASS()
class RTS_PROJECT_API ARTSAIController : public AAIController
{
	GENERATED_BODY()
	
protected:
	ARTSAIController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

protected:
	virtual void PostInitializeComponents() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

public:
	class ACommander * GetLeadRTSObject();
	void ClearCommander();
	void SetCommander(ACommander * Commander);

	UObject* GetBlackBoardKeyAsObject(const FName InKeyName) const;

public:
	FORCEINLINE FAIRequestID GetAIRequestId() const { return AIRequestId; }
	FORCEINLINE FAIRequestID GetAIAbortId() const { return AIAbortID; }
	void SendAIMessage(const FName AIMessage, FAIMessage::EStatus Status, EAIMessageType MessageType);

	bool IsAbortingTask() const;

public:
	virtual void EnqueueOrder(URTSOrder* InOrder, bool InbIsEnquedOrder = false);

	virtual URTSOrder* GetCurrentOrder() const;

	template<class T>
	T* GetCurrentOrder() const { return Cast<T>(GetCurrentOrder()); }

	virtual URTSOrder* GetAbortingOrder() const;

	/*Called from the Tree*/
	virtual void OnOrderFinished(UBTTaskNode * InTaskNode, const bool InOrderSuccess);


protected:
	virtual void ClearOrders();
	virtual void SetCurrentOrder(URTSOrder* InOrder);

protected:
	virtual void ActorsPerceptionUpdated(const TArray<AActor*>& UpdatedActors) override;

protected:
	UFUNCTION()
	virtual void OnTargetPerceptionUpdated(AActor * Actor, FAIStimulus Stimulus);

	virtual bool ConfigureRTSPerception(class ARTSMinion* Minion);
	virtual bool ConfigureAISense(class ARTSMinion* InMinion, UAISenseConfig* InSenseConfig);


public:
	static const FName AIMessageOrderRequest;
	static const FName AIMessageAbortRequest;
	static const FName AIGuardLocationKey;
	static const FName AICurrentOrderKey;

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

private:

	FRTSAIPerceptionConfig DefaultPerceptionConfig;

	FAIRequestID AIRequestId;
	FORCEINLINE void StoreAIRequestId(FAIRequestID& InID ) { InID = InID + 1; }

	FAIRequestID AIAbortID;

protected:
	/*May have to convert this to TArray, due to Object cleanup, will do during queued order implementation*/
	TQueue<URTSOrder*> EnquedOrders;

	int NumOrders = 0;

	UPROPERTY(Transient)
	URTSOrder* CurrentOrder = nullptr;


	UPROPERTY(Transient)
	URTSOrder* AbortingOrder = nullptr;
};
