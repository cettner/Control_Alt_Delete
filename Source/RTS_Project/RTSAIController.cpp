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

void ARTSAIController::OnPossess(APawn * InPawn)
{
	Super::OnPossess(InPawn);

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

AActor * ARTSAIController::GetTarget()
{
	return(Cast<AActor>(BlackboardComp->GetValueAsObject(Target)));
}

void ARTSAIController::ClearTarget()
{
	BlackboardComp->ClearValue(Target);
}

void ARTSAIController::SetCommander(ACommander * Commander)
{
	BlackboardComp->SetValueAsObject("OwningCommander", Commander);
}

ACommander * ARTSAIController::GetCommander()
{
	return(Cast<ACommander>(BlackboardComp->GetValueAsObject("OwningCommander")));
}

void ARTSAIController::ClearCommander()
{
	BlackboardComp->ClearValue("OwningCommander");
}

