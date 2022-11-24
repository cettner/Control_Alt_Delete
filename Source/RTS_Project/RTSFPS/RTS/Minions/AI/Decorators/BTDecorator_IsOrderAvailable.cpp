// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_IsOrderAvailable.h"
#include "RTS_Project/RTSFPS/GameSystems/HealthSystem/HealthComponent.h"
#include "../RTSAIController.h"

#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

UBTDecorator_IsOrderAvailable::UBTDecorator_IsOrderAvailable()
{
	NodeName = "Is Order Available";
}


bool UBTDecorator_IsOrderAvailable::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	//const ARTSAIController * controller = Cast<ARTSAIController>(OwnerComp.GetAIOwner());
	//const bool retval = controller->IsOrderAvailable();
	return(false);
}
