// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RTS_Project/RTSFPS/RTS/Structures/RTSStructure.h"

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_FindNearestStructure.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UBTService_FindNearestStructure : public UBTService_BlackboardBase
{
	GENERATED_BODY()


protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual ARTSStructure * GetBestStructure(APawn* Minion, TArray<ARTSStructure*> structures) const;

protected:
		UPROPERTY(EditDefaultsOnly)
		bool bMatchTeam = true;

	    UPROPERTY(EditDefaultsOnly)
		bool bNeedDropPoint = true;

};
