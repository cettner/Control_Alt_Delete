// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "GenericTeamAgentInterface.h"

#include "BTService_UpdatePercieveKey.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UBTService_UpdatePercieveKey : public UBTService_BlackboardBase
{
	GENERATED_BODY()

	UBTService_UpdatePercieveKey();
	
	protected:
	    virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

    protected:
		virtual AActor * GetClosestTarget(AActor * ControlledPawn, TArray<AActor*> PercievedActors);

	protected:
		UPROPERTY(EditAnywhere, Category = Perception)
		TSubclassOf<AActor> FilterClass = nullptr;

		UPROPERTY(EditAnywhere, Category = Perception)
		TEnumAsByte<ETeamAttitude::Type> TeamPerceptionFilter = ETeamAttitude::Hostile;

		UPROPERTY(EditAnywhere, Category = Perception)
		bool bUseClosest = true;

};
