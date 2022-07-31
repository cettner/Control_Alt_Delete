// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_FindNearestStructure.h"
#include "RTS_Project/RTSFPS/GameArchitecture/RTFPSGameState.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "AIController.h"

void UBTService_FindNearestStructure::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	UWorld* World = GetWorld();
	if (Controller == nullptr || World == nullptr) return;

	ARTSMinion * Minion = Controller->GetPawn<ARTSMinion>();
	ARTFPSGameState* GS = World->GetGameState<ARTFPSGameState>();
	if (Minion == nullptr || GS == nullptr) return;

	TArray<ARTSStructure *> teamstructures =  GS->GetAllStructuresOfTeam(Minion->GetTeam());

	ARTSStructure* closeststructure = GetBestStructure(Minion,teamstructures);

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), closeststructure);
}

ARTSStructure* UBTService_FindNearestStructure::GetBestStructure(APawn* Minion, TArray<ARTSStructure*> structures) const
{
	float shortestdistance = MAX_FLT;
	ARTSStructure * beststructure = nullptr;

	for (int i = 0; i < structures.Num(); i++)
	{
		float distanceto = structures[i]->GetDistanceTo(Minion);
		if (structures[i]->IsDropPointFor(AResource::StaticClass()) && distanceto < shortestdistance)
		{
			beststructure = structures[i];
			shortestdistance = distanceto;
		}
	}

	return beststructure;
}
