// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_UpdateGuardLocationKey.h"

#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "AIController.h"

void UBTService_UpdateGuardLocationKey::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (Controller == nullptr) return;

	APawn* Minion = Controller->GetPawn();
	if (Minion == nullptr) return;

	OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), Minion->GetActorLocation());
}
