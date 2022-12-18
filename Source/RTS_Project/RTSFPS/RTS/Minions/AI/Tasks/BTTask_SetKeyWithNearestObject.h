// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "../../../../GameArchitecture/TeamResourceState.h"
#include "BTTask_SetKeyWithNearestObject.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UBTTask_SetKeyWithNearestObject : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
	UBTTask_SetKeyWithNearestObject();
	
	protected:
		virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	protected:
		virtual AActor* GetNeareastTeamActor(const ARTSMinion* InPawn, const ATeamResourceState * InTeam) const;
		virtual TArray<AActor*> GetTeamActorList(const ATeamResourceState* InTeamState) const;

	protected:
		UPROPERTY(EditAnywhere, meta = (MustImplement = "RTSObjectInterface"))
		TSubclassOf<AActor> TeamObjectClass = nullptr;
};
