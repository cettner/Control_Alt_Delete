// Fill out your copyright notice in the Description page of Project Settings.

#include "RTSAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "RTSFPS/BaseClasses/RTSMinion.h"
#include "RTSFPS/FPS/Commander.h"

const FName ARTSAIController::AIMessage_Finished = TEXT("Task Complete");

ARTSAIController::ARTSAIController()
{
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
	BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));
	AIRequestId = 1U;
}

void ARTSAIController::OnPossess(APawn * InPawn)
{
	Super::OnPossess(InPawn);

	ARTSMinion *Minion = Cast<ARTSMinion>(InPawn);

	if (Minion != NULL && Minion->GetBehavior() != NULL)
	{
		BlackboardComp->InitializeBlackboard(*Minion->GetBehavior()->BlackboardAsset);
		BehaviorComp->StartTree(*Minion->GetBehavior());
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

void ARTSAIController::SendAIMessage(const FName AIMessage, FAIMessage::EStatus Status)
{
	FAIMessage Msg(AIMessage, this, AIRequestId, Status);
	FAIMessage::Send(this, Msg);
	StoreAIRequestId();
}

ACommander * ARTSAIController::GetCommander()
{
	return(Cast<ACommander>(BlackboardComp->GetValueAsObject("OwningCommander")));
}

void ARTSAIController::ClearCommander()
{
	BlackboardComp->ClearValue("OwningCommander");
}

