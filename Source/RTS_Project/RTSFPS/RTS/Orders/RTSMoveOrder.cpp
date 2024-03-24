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
	bool ismultigoal = false;
	if (InResult->Items.Num() >= 2)
	{
		const float firstscore = InResult->GetItemScore(0);
		const float secondscore = InResult->GetItemScore(1);
		ismultigoal = (firstscore >= 1.0f) && (secondscore >= 1.0f);
	}
	if (ismultigoal)
	{
		int debug = 9;
	}

	TArray<FVector> outvectors = TArray<FVector>();
	InResult->GetAllAsLocations(outvectors);

	const int32 smallestnum = (outvectors.Num() > AssignedUnits.Num()) ? outvectors.Num() : AssignedUnits.Num();
	for (int32 i = 0; i < smallestnum; i++)
	{
		for (int32 k = i; k < AssignedUnits.Num(); k++)
		{
			if (ARTSMinion* minion = Cast<ARTSMinion>(AssignedUnits[k].GetObject()))
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
