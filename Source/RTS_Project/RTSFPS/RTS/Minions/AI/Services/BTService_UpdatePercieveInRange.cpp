// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_UpdatePercieveInRange.h"
#include "RTS_Project/RTSFPS/RTS/RTSMinion.h"
#include "RTS_Project/RTSFPS/RTS/Minions/AI/RTSAIController.h"

#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"


AActor* UBTService_UpdatePercieveInRange::GetBestTarget(APawn* ControlledPawn, TArray<AActor*> PercievedActors, UBehaviorTreeComponent& OwnerComp) const
{
	AActor* retval = Super::GetBestTarget(ControlledPawn, PercievedActors, OwnerComp);

	if(retval != nullptr)
	{
		const UBlackboardComponent* blackboard = OwnerComp.GetBlackboardComponent();
		const AAIController* controller = OwnerComp.GetAIOwner();
		
		const ARTSMinion* minion = Cast<ARTSMinion>(controller->GetPawn());
		const float straydistance = minion->GetMinionStrayDistance();


		if (straydistance >= 0.0f)
		{
			const FVector guardlocation = blackboard->GetValueAsVector(GuardLocationKey.SelectedKeyName);
			const FVector targetlocation = retval->GetActorLocation();
			const float currentdistance = (targetlocation - guardlocation).Size();
			if (straydistance <= currentdistance)
			{
				retval = nullptr;
			}
		}

	}
	return retval;
}
