// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GenericTeamAgentInterface.h"
#include "RTSAIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BrainComponent.h"
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



UCLASS()
class RTS_PROJECT_API ARTSAIController : public AAIController
{
	GENERATED_BODY()
	
public:

	ARTSAIController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void OnPossess(APawn *InPawn) override;
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

public:
	class ACommander * GetCommander();
	void ClearCommander();
	void SetCommander(ACommander * Commander);

	FORCEINLINE FAIRequestID GetAIRequestId() const { return AIRequestId; }
	void SendAIMessage(const FName AIMessage, FAIMessage::EStatus Status);

public:
	virtual void ReleaseAssets();
	void SetTarget(AActor* newtarget);
	AActor* GetTarget();
	void ClearTarget();

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
	FName Target = "Target";

private:

	FRTSAIPerceptionConfig DefaultPerceptionConfig;

	FAIRequestID AIRequestId;
	FORCEINLINE void StoreAIRequestId() { AIRequestId = AIRequestId + 1; }

};
