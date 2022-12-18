// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetKeyWithNearestObject.h"
#include "../../../../GameArchitecture/RTFPSGameState.h"


UBTTask_SetKeyWithNearestObject::UBTTask_SetKeyWithNearestObject()
{
	const FString KeyName = GetSelectedBlackboardKey().ToString();

	NodeName = "Set " + KeyName + " With Nearest Object";
}

EBTNodeResult::Type UBTTask_SetKeyWithNearestObject::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type retval = EBTNodeResult::Failed;

	const AAIController* controller = OwnerComp.GetAIOwner();
	const UWorld* world = GetWorld();
	const ARTSMinion* minion = controller->GetPawn<ARTSMinion>();
	const ARTFPSGameState* gs = world->GetGameState<ARTFPSGameState>();
	const ATeamResourceState* ts = gs->GetTeamState<ATeamResourceState>(minion->GetTeam());
	
	AActor* nearestteamactor = GetNeareastTeamActor(minion, ts);
	if (IsValid(nearestteamactor))
	{
		UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();
		blackboard->SetValueAsObject(GetSelectedBlackboardKey(), nearestteamactor);
		retval = EBTNodeResult::Succeeded; 
	}
	
	return retval;
}

AActor* UBTTask_SetKeyWithNearestObject::GetNeareastTeamActor(const ARTSMinion* InPawn, const ATeamResourceState* InTeam) const
{
	AActor* retval = nullptr;
	float shortestdistance = MAX_FLT;
	const TArray<AActor*> actorlist = GetTeamActorList(InTeam);

	for (int i = 0; i < actorlist.Num(); i++)
	{
		const float distanceto = actorlist[i]->GetSquaredDistanceTo(InPawn);
		if (distanceto < shortestdistance)
		{
			retval = actorlist[i];
			shortestdistance = distanceto;
		}
	}

	return retval;
}

TArray<AActor *> UBTTask_SetKeyWithNearestObject::GetTeamActorList(const ATeamResourceState* InTeamState) const
{
	TArray<AActor*> retval = TArray<AActor*>();
	if (TeamObjectClass != nullptr)
	{
		const bool bsearchminions = TeamObjectClass.GetDefaultObject()->IsA(ARTSMinion::StaticClass());
		if (bsearchminions)
		{
			const TArray<ARTSMinion*> minions = InTeamState->GetMinions();
			for (int i = 0; i < minions.Num(); i++)
			{
				if (minions[i]->IsA(TeamObjectClass))
				{
					retval.Emplace(minions[i]);
				}
			}
		}
		else if(const bool bsearchstructures = TeamObjectClass.GetDefaultObject()->IsA(ARTSStructure::StaticClass()))
		{
			const TArray<ARTSStructure*> structures = InTeamState->GetStructures();
			for (int i = 0; i < structures.Num(); i++)
			{
				if (structures[i]->IsA(TeamObjectClass))
				{
					retval.Emplace(structures[i]);
				}
			}
		}
	}

	return retval;
}
