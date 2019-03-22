// Fill out your copyright notice in the Description page of Project Settings.

#include "RTSAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "RTSMinion.h"
#include "Commander.h"

ARTSAIController::ARTSAIController()
{
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
	BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));
}

void ARTSAIController::Possess(APawn * InPawn)
{
	Super::Possess(InPawn);

	ARTSMinion *Minion = Cast<ARTSMinion>(InPawn);

	if (Minion != NULL && Minion->RTSBehavior != NULL)
	{
		BlackboardComp->InitializeBlackboard(*Minion->RTSBehavior->BlackboardAsset);
		BehaviorComp->StartTree(*Minion->RTSBehavior);
	}
}

void ARTSAIController::SetTarget(AActor * newtarget)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsObject(Target, newtarget);
	}
}

ACommander * ARTSAIController::GetCommander()
{
	return(Cast<ACommander>(BlackboardComp->GetValueAsObject("OwningCommander")));
}

