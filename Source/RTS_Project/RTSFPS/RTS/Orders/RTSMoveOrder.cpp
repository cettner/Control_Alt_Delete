// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSMoveOrder.h"
#include "RTSOrderGroup.h"
#include "RTS_Project/RTSFPS/RTS/RTSMinion.h"
#include "RTS_Project/RTSFPS/RTS/Minions/AI/RTSAIController.h"

URTSMoveOrder::URTSMoveOrder() : Super()
{
	PropertyName = TEXT("Move Unit");
	PropertyDescription = FText::FromString("Move the Selected Units to The Target Position");
}

void URTSMoveOrder::LoadAIBlackBoard(UBlackboardComponent* InBlackBoard) const
{
	const FVector& movelocation = GetOrderGroup()->GetOrderContext().GetContextPoint();
	InBlackBoard->SetValueAsVector(TEXT("MoveLocation"), movelocation);
}

void URTSMoveOrder::UpdateUnitsWithResults(TSharedPtr<FEnvQueryResult>& InResult)
{
	TArray<FVector> outvectors = TArray<FVector>();
	InResult->GetAllAsLocations(outvectors);

	const int32 smallestnum = (outvectors.Num() < AssignedUnits.Num()) ? outvectors.Num() : AssignedUnits.Num();
	for (int32 i = 0; i < smallestnum; i++)
	{
		for (const TScriptInterface<IRTSObjectInterface>& unit : AssignedUnits)
		{
			if (const ARTSMinion* minion = Cast<ARTSMinion>(unit.GetObject()))
			{
				const ARTSAIController * aic = minion->GetController<ARTSAIController>();
				const FVector& relativemovelocation = outvectors[i];
				UBlackboardComponent * blackboard = aic->GetMinonBlackBoard();
				blackboard->SetValueAsVector(TEXT("MoveLocation"), relativemovelocation);
				break;
			}
		}
	}


}
