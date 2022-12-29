// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_UpdatePercieveKey.h"

#include "AIController.h"
#include "Perception/AISense_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "Perception/AIPerceptionComponent.h"

UBTService_UpdatePercieveKey::UBTService_UpdatePercieveKey()
{
	NodeName = "Update Key With Perception";
	PerceptionSense = UAISense_Sight::StaticClass();
}

void UBTService_UpdatePercieveKey::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	if (Controller == nullptr) return;
	
	APawn* Minion = Controller->GetPawn();
	if (Minion == nullptr) return;

	TArray<AActor*> percievedactors = TArray<AActor*>();

	switch (TeamPerceptionFilter)
	{
	case ETeamAttitude::Friendly :
		GetFriendlyActors(Controller, percievedactors);
		break;
	case ETeamAttitude::Hostile :
		Controller->GetPerceptionComponent()->GetHostileActors(percievedactors);
		break;
	case ETeamAttitude::Neutral :
		GetNeutralActors(Controller, percievedactors);
		break;
	default:
		Controller->GetPerceptionComponent()->GetCurrentlyPerceivedActors(PerceptionSense, percievedactors);
		break;
	}

	AActor* ClosestTarget = GetBestTarget(Minion, percievedactors);
	OwnerComp.GetBlackboardComponent()->SetValueAsObject(GetSelectedBlackboardKey(), ClosestTarget);
}
	
AActor * UBTService_UpdatePercieveKey::GetBestTarget(APawn* ControlledPawn, TArray<AActor*> PercievedActors) const
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

void UBTService_UpdatePercieveKey::GetFriendlyActors(const AAIController * InController, TArray<AActor*>& OutActors) const
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
			if (!IsValid(OutActors[i]) || InController->GetTeamAttitudeTowards(*OutActors[i]) !=  ETeamAttitude::Friendly)
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

void UBTService_UpdatePercieveKey::GetNeutralActors(const AAIController* InController, TArray<AActor*>& OutActors) const
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
