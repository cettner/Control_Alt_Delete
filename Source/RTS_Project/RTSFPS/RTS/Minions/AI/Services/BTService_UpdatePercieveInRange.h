// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../BTService_UpdatePercieveKey.h"
#include "BTService_UpdatePercieveInRange.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UBTService_UpdatePercieveInRange : public UBTService_UpdatePercieveKey
{
	GENERATED_BODY()
	
	protected:
		virtual AActor* GetBestTarget(APawn* ControlledPawn, TArray<AActor*> PercievedActors, UBehaviorTreeComponent& OwnerComp) const override;

	protected:
		UPROPERTY(EditAnywhere, Category = Blackboard)
		FBlackboardKeySelector GuardLocationKey;
};
