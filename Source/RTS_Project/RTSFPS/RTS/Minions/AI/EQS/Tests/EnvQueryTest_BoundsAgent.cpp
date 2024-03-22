// Fill out your copyright notice in the Description page of Project Settings.


#include "EnvQueryTest_BoundsAgent.h"
#include "../Context/EnvQueryItemType_Order.h"
#include "RTS_Project/RTSFPS/RTS/Minions/AI/EQS/Interface/QueryBoundsInterface.h"

UEnvQueryTest_BoundsAgent::UEnvQueryTest_BoundsAgent(const FObjectInitializer& InObjectInitializer) : Super(InObjectInitializer)
{
	Cost = EEnvTestCost::Low;
	ValidItemType = UEnvQueryItemType_VectorBase::StaticClass();

	SetWorkOnFloatValues(true);
}

void UEnvQueryTest_BoundsAgent::RunTest(FEnvQueryInstance& QueryInstance) const
{
	UObject* QueryOwner = QueryInstance.Owner.Get();

	if (QueryOwner == nullptr)
	{
		return;
	}
	 
	FloatValueMin.BindData(QueryOwner, QueryInstance.QueryID);
	const float MinThresholdValue = FloatValueMin.GetValue();

	FloatValueMax.BindData(QueryOwner, QueryInstance.QueryID);
	const float MaxThresholdValue = FloatValueMax.GetValue();

	TArray<AActor*> ContextActors = TArray<AActor*>();
	TArray<FVector> ContextLocations = TArray<FVector>();
	QueryInstance.PrepareContext(Context, ContextActors);
	if(!QueryInstance.PrepareContext(Context, ContextLocations))
	{
		return;
	}

	if (ContextActors.Num() && (Cast<IQueryBoundsInterface>(ContextActors[0]) != nullptr))
	{
		for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
		{
			const FVector ItemLocation = GetItemLocation(QueryInstance, It.GetIndex());
			for (int32 ContextIndex = 0; ContextIndex < ContextActors.Num(); ContextIndex++)
			{
				const IQueryBoundsInterface* boundinginterface = Cast<IQueryBoundsInterface>(ContextActors[ContextIndex]);
				const bool isinbounds = boundinginterface->IsPointWithinContextBounds(ItemLocation, Context);


				if (isinbounds)
				{
					//It.SetScore(TestPurpose, FilterType, isinbounds, true);
					It.SetScore(TestPurpose, FilterType, 0.0f, MinThresholdValue, MaxThresholdValue);
				}
				else
				{
					float distance = NAN;
					for (int32 It2 = 0; It2 < ContextLocations.Num(); ++It2) 
					{
						const FVector ContextItemLoc = ContextLocations[It2];
						distance = FVector::Dist(ItemLocation, ContextItemLoc);
					}
					It.SetScore(TestPurpose, FilterType, distance, MinThresholdValue, MaxThresholdValue);
				}

			}
		}
	}
	else
	{
		for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
		{
			const FVector ItemLocation = GetItemLocation(QueryInstance, It.GetIndex());
			float distance = NAN;
			for (int32 It2 = 0; It2 < ContextLocations.Num(); ++It2) 
			{
				const FVector ContextItemLoc = ContextLocations[It2];
				distance = FVector::Dist(ItemLocation, ContextItemLoc);
			}
			It.SetScore(TestPurpose, FilterType, distance, MinThresholdValue, MaxThresholdValue);
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
