// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "GenericTeamAgentInterface.h"
#include "Perception/AISense.h"

#include "BTTask_UpdatePercieveKey.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UBTTask_UpdatePercieveKey : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	virtual AActor* GetBestTarget(APawn* ControlledPawn, TArray<AActor*> PercievedActors) const;
	virtual void GetFriendlyActors(const AAIController* InController, TArray<AActor*>& OutActors) const;
	virtual void GetNeutralActors(const AAIController* InComp, TArray<AActor*>& OutActors) const;

protected:
	UPROPERTY(EditAnywhere, Category = Perception)
	TSubclassOf<AActor> FilterClass = nullptr;

	UPROPERTY(EditAnywhere, Category = Perception)
	TEnumAsByte<ETeamAttitude::Type> TeamPerceptionFilter = ETeamAttitude::Hostile;

	UPROPERTY(EditAnywhere, Category = Perception)
	bool bUseClosest = true;

	UPROPERTY(EditAnyWhere, Category = Perception)
	TSubclassOf<UAISense> PerceptionSense = nullptr;
};
