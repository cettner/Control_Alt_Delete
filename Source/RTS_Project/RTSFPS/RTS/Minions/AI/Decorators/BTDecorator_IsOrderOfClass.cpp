// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_IsOrderOfClass.h"
#include "../RTSAIController.h"


UBTDecorator_IsOrderOfClass::UBTDecorator_IsOrderOfClass()
{
	NodeName = "Is Order Of Class";
}


bool UBTDecorator_IsOrderOfClass::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool retval = false;
	const ARTSAIController * controller = Cast<ARTSAIController>(OwnerComp.GetAIOwner());
	const URTSOrder * currentorder = controller->GetCurrentOrder();

	if (currentorder != nullptr)
	{
		retval = currentorder->IsA(OrderCheckClass);
	}

	return(retval);
}
