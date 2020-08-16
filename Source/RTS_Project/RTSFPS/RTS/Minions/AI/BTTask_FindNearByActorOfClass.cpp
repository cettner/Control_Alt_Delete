// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindNearByActorOfClass.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "AIController.h"
#include "EngineUtils.h"

EBTNodeResult::Type UBTTask_FindNearByActorOfClass::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* Controller = OwnerComp.GetAIOwner();
	AActor* FoundActor = nullptr;
	float mindistance = MAX_FLT;
	EBTNodeResult::Type Retval = EBTNodeResult::Failed;


	if (Controller)
	{
		APawn* mypawn = Controller->GetPawn();
		if (mypawn)
		{
			for (TActorIterator<AActor> It(GetWorld(), SearchClass); It; ++It)
			{
				AActor* currentactor = *It;
				float distanceto = currentactor->GetDistanceTo(mypawn);
				if (distanceto <= SearchDistance && distanceto < mindistance)
				{
					FoundActor = currentactor;
					mindistance = distanceto;

					/*If we found one and dont care if its the closest one leave the loop*/
					if (!bfindClosest)
					{
						break;
					}
				}
			}
		}
	}


	if (FoundActor)
	{
		OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>(GetSelectedBlackboardKey(), FoundActor);
		Retval = EBTNodeResult::Succeeded;
	}



	return Retval;
}
