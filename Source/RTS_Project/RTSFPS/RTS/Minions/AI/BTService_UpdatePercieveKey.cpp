// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_UpdatePercieveKey.h"

#include "AIController.h"
#include "Perception/AISense_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

UBTService_UpdatePercieveKey::UBTService_UpdatePercieveKey()
{
	NodeName = "Update Key With Perception";
}

void UBTService_UpdatePercieveKey::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (Controller == nullptr) return;
	
	APawn* Minion = Controller->GetPawn();
	if (Minion == nullptr) return;

	TArray<AActor*> PercievedActors;
	//Controller->GetPerceptionComponent()->GetPerceivedActors(UAISense_Sight::StaticClass(), PercievedActors);
	Controller->GetPerceptionComponent()->GetHostileActors(PercievedActors);

	AActor* ClosestTarget = GetClosestTarget(Minion, PercievedActors);
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), ClosestTarget);
}
	
	
AActor * UBTService_UpdatePercieveKey::GetClosestTarget(AActor* ControlledPawn, TArray<AActor*> PercievedActors)
{
	AActor* Bestactor = nullptr;
	float shortestdistance = MAX_FLT;

	if (PercievedActors.Num() > 0)
	{
		Bestactor = PercievedActors[0];
		shortestdistance = Bestactor->GetDistanceTo(ControlledPawn);

		for (int i = 1; i < PercievedActors.Num(); i++)
		{
			float newdistance = PercievedActors[i]->GetDistanceTo(ControlledPawn);
			if (newdistance < shortestdistance)
			{
				Bestactor = PercievedActors[i];
				shortestdistance = newdistance;
			}
		}
	}

	return Bestactor;
}
