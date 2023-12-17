// Fill out your copyright notice in the Description page of Project Settings.


#include "EnvQueryTest_BoundsAgent.h"

#include "EnvironmentQuery/Items/EnvQueryItemType_VectorBase.h"
#include "RTS_Project/RTSFPS/RTS/Minions/AI/EQS/Interface/QueryBoundsInterface.h"

UEnvQueryTest_BoundsAgent::UEnvQueryTest_BoundsAgent(const FObjectInitializer& InObjectInitializer) : Super(InObjectInitializer)
{
	Cost = EEnvTestCost::Low;
	ValidItemType = UEnvQueryItemType_VectorBase::StaticClass();
	/*All this test can do is filter currently*/
	SetWorkOnFloatValues(false);
}

void UEnvQueryTest_BoundsAgent::RunTest(FEnvQueryInstance& QueryInstance) const
{
	UObject* QueryOwner = QueryInstance.Owner.Get();
	QueryInstance.ContextCache.Find(Context);

	if (QueryOwner == nullptr)
	{
		return;
	}
	 
	FloatValueMin.BindData(QueryOwner, QueryInstance.QueryID);
	const float MinThresholdValue = FloatValueMin.GetValue();

	FloatValueMax.BindData(QueryOwner, QueryInstance.QueryID);
	const float MaxThresholdValue = FloatValueMax.GetValue();

	TArray<AActor*> ContextActors;
	if (!QueryInstance.PrepareContext(Context, ContextActors))
	{
		return;
	}

	for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
	{
		const FVector ItemLocation = GetItemLocation(QueryInstance, It.GetIndex());
		for (int32 ContextIndex = 0; ContextIndex < ContextActors.Num(); ContextIndex++)
		{
			IQueryBoundsInterface * boundinginterface = Cast<IQueryBoundsInterface>(ContextActors[ContextIndex]);
			bool isinbounds = boundinginterface->IsPointWithinContextBounds(ItemLocation, Context);
			
			if (GetWorkOnFloatValues())
			{
				const float boundsscore = isinbounds ? MaxThresholdValue : MinThresholdValue;
				It.SetScore(TestPurpose, FilterType, boundsscore, MinThresholdValue, MaxThresholdValue);
			}
			else
			{
				It.SetScore(TestPurpose, FilterType, isinbounds, true);
			}
			
			if (isinbounds)
			{

			}
			else
			{
				/*The point fails*/
				It.ForceItemState(EEnvItemStatus::Failed);
			}
		}
	}

}

FText UEnvQueryTest_BoundsAgent::GetDescriptionTitle() const
{
	FString basetitle = FString::Printf(TEXT("Filter Valid Bounds of %s"), *UEnvQueryTypes::DescribeContext(Context).ToString());
	return FText::FromString(basetitle);
}

FText UEnvQueryTest_BoundsAgent::GetDescriptionDetails() const
{
	return FText();
}
