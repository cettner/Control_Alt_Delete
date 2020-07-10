// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_IsTargetAlive.h"
#include "RTS_Project/RTSFPS/GameSystems/HealthSystem/HealthComponent.h"

#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"


bool UBTDecorator_IsTargetAlive::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	bool isAlive = false;
	AActor* target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>("Target"));

	if (target)
	{
		UHealthComponent* TargetHealth = Cast<UHealthComponent>(target->FindComponentByClass(UHealthComponent::StaticClass()));
		if (TargetHealth)
		{
			isAlive = TargetHealth->IsAlive();
		}
	}
	return(isAlive);
}
