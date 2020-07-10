// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_CheckForCommand.h"
#include "RTSAIController.h"
#include "RTS_Project/RTSFPS/BaseClasses/RTSMinion.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"





UBTService_CheckForCommand::UBTService_CheckForCommand()
{
	bCreateNodeInstance = true;
}

void UBTService_CheckForCommand::TickNode(UBehaviorTreeComponent& OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	ARTSAIController *RTSPC = Cast<ARTSAIController>(OwnerComp.GetAIOwner());

	if (RTSPC != NULL)
	{
		ARTSMinion *Minion = Cast<ARTSMinion>(GetWorld()->GetFirstPlayerController()->GetPawn());
	}
}
