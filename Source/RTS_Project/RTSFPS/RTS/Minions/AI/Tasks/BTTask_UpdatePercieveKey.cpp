// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_UpdatePercieveKey.h"

#include "AIController.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Perception/AIPerceptionComponent.h"


EBTNodeResult::Type UBTTask_UpdatePercieveKey::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	APawn* Minion = Controller->GetPawn();
	TArray<AActor*> percievedactors = TArray<AActor*>();

	EBTNodeResult::Type retval = EBTNodeResult::Failed;

	switch (TeamPerceptionFilter)
	{
	case ETeamAttitude::Friendly:
		GetFriendlyActors(Controller, percievedactors);
		break;
	case ETeamAttitude::Hostile:
		Controller->GetPerceptionComponent()->GetHostileActors(percievedactors);
		break;
	case ETeamAttitude::Neutral:
		GetNeutralActors(Controller, percievedactors);
		break;
	default:
		Controller->GetPerceptionComponent()->GetPerceivedActors(PerceptionSense, percievedactors);
		break;
	}

	AActor* ClosestTarget = GetBestTarget(Minion, percievedactors);

	if (ClosestTarget != nullptr)
	{
		retval = EBTNodeResult::Succeeded;
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), ClosestTarget);
	return retval;
}

AActor* UBTTask_UpdatePercieveKey::GetBestTarget(APawn* ControlledPawn, TArray<AActor*> PercievedActors) const
{
	AActor* Bestactor = nullptr;
	float shortestdistance = MAX_FLT;

	if (PercievedActors.Num() > 0)
	{
		Bestactor = PercievedActors[0];
		shortestdistance = Bestactor->GetSquaredDistanceTo(ControlledPawn);

		for (int i = 1; i < PercievedActors.Num(); i++)
		{
			float newdistance = PercievedActors[i]->GetSquaredDistanceTo(ControlledPawn);
			if (newdistance < shortestdistance)
			{
				Bestactor = PercievedActors[i];
				shortestdistance = newdistance;
			}
		}
	}

	return Bestactor;
}

void UBTTask_UpdatePercieveKey::GetFriendlyActors(const AAIController* InController, TArray<AActor*>& OutActors) const
{
	const UAIPerceptionComponent* perceptioncomp = InController->GetAIPerceptionComponent();

	/*Stolen from GetHostileActors() returns true if any of the actors are invalid*/
	const bool bDeadDataFound = perceptioncomp->GetFilteredActors([]
	(const FActorPerceptionInfo& ActorPerceptionInfo)
		{
			return (!ActorPerceptionInfo.bIsHostile && ActorPerceptionInfo.HasAnyKnownStimulus());
		}, OutActors);


	int endindex = OutActors.Num();
	if (bDeadDataFound)
	{
		for (int32 i = 0; i < OutActors.Num(); i++)
		{
			if (!IsValid(OutActors[i]) || InController->GetTeamAttitudeTowards(*OutActors[i]) != ETeamAttitude::Friendly)
			{
				OutActors.RemoveAt(i);
				endindex--;
				i--;
			}
		}
	}
	else
	{
		for (int32 i = 0; i < OutActors.Num(); i++)
		{
			if (InController->GetTeamAttitudeTowards(*OutActors[i]) != ETeamAttitude::Friendly)
			{
				OutActors.RemoveAt(i);
				endindex--;
				i--;
			}
		}
	}
}

void UBTTask_UpdatePercieveKey::GetNeutralActors(const AAIController* InController, TArray<AActor*>& OutActors) const
{
	const UAIPerceptionComponent* perceptioncomp = InController->GetAIPerceptionComponent();

	/*Stolen from GetHostileActors() returns true if any of the actors are invalid*/
	const bool bDeadDataFound = perceptioncomp->GetFilteredActors([]
	(const FActorPerceptionInfo& ActorPerceptionInfo)
		{
			return (!ActorPerceptionInfo.bIsHostile && ActorPerceptionInfo.HasAnyKnownStimulus());
		}, OutActors);


	int endindex = OutActors.Num();
	if (bDeadDataFound)
	{
		for (int32 i = 0; i < OutActors.Num(); i++)
		{
			if (!IsValid(OutActors[i]) || InController->GetTeamAttitudeTowards(*OutActors[i]) != ETeamAttitude::Neutral)
			{
				OutActors.RemoveAt(i);
				endindex--;
				i--;
			}
		}
	}
	else
	{
		for (int32 i = 0; i < OutActors.Num(); i++)
		{
			if (InController->GetTeamAttitudeTowards(*OutActors[i]) != ETeamAttitude::Neutral)
			{
				OutActors.RemoveAt(i);
				endindex--;
				i--;
			}
		}
	}

}
