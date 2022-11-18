// Fill out your copyright notice in the Description page of Project Settings.


#include "RTSOrder.h"
#include "../../Shared/Interfaces/RTSObjectInterface.h"

TArray<TScriptInterface<IRTSObjectInterface>> URTSOrder::GetBestMinionsForOrder(const TArray<TScriptInterface<IRTSObjectInterface>>& InMinionSet, const FHitResult& InTarget) const
{
	const int applicationcount = GetApplicationCount();
	const bool canapplytocount = (InMinionSet.Num() >= applicationcount);
	TArray<TScriptInterface<IRTSObjectInterface>> retval = TArray<TScriptInterface<IRTSObjectInterface>>();


	/*Determine if the number of units ordered needs to be an exact amount or specific unit*/
	int iterationcount = -1;
	if (UsesExactApplicationCount() && canapplytocount)
	{
		for (int i = 0; i < applicationcount; i++)
		{
			retval.Emplace(InMinionSet[i]);
		}
	}
	else if (canapplytocount == true)
	{
		retval = TArray<TScriptInterface<IRTSObjectInterface>>(InMinionSet);
	}

	return retval;
}

void URTSOrder::LoadAIBlackBoard(UBlackboardComponent* InBlackBoard) const
{

}
