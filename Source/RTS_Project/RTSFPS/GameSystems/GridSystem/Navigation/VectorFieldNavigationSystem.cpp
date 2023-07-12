// Fill out your copyright notice in the Description page of Project Settings.


#include "VectorFieldNavigationSystem.h"

#include "EngineUtils.h"


FVectorFieldQuery::FVectorFieldQuery(const UObject* InOwner, const AGameGrid& InNavData, const FVector& Start, const FVector& End, FSharedConstNavQueryFilter SourceQueryFilter, FNavPathSharedPtr InPathInstanceToFill, const float CostLimit) :
	FPathFindingQueryData(InOwner, Start, End, SourceQueryFilter, 0 /*InNavDataFlags*/, true /*bInAllowPartialPaths*/, CostLimit),
	NavData(&InNavData), PathInstanceToFill(InPathInstanceToFill), NavAgentProperties(InNavData.GetConfig())
{
	if (!QueryFilter.IsValid() && NavData.IsValid())
	{
		QueryFilter = NavData->GetDefaultQueryFilter();
	}
}

FVectorFieldQuery::FVectorFieldQuery(const INavAgentInterface& InNavAgent, const AGameGrid& InNavData, const FVector& Start, const FVector& End, FSharedConstNavQueryFilter SourceQueryFilter, FNavPathSharedPtr InPathInstanceToFill, const float CostLimit) :
	FPathFindingQueryData(Cast<UObject>(&InNavAgent), Start, End, SourceQueryFilter, 0 /*InNavDataFlags*/, true /*bInAllowPartialPaths*/, CostLimit),
	NavData(&InNavData), PathInstanceToFill(InPathInstanceToFill), NavAgentProperties(InNavAgent.GetNavAgentPropertiesRef())
{
	if (!QueryFilter.IsValid() && NavData.IsValid())
	{
		QueryFilter = NavData->GetDefaultQueryFilter();
	}
}

FVectorFieldQuery::FVectorFieldQuery(const FVectorFieldQuery& Source) :
	FPathFindingQueryData(Source.Owner.Get(), Source.StartLocation, Source.EndLocation, Source.QueryFilter, Source.NavDataFlags, Source.bAllowPartialPaths, Source.CostLimit),
	NavData(Source.NavData), PathInstanceToFill(Source.PathInstanceToFill), NavAgentProperties(Source.NavAgentProperties)
{
	if (!QueryFilter.IsValid() && NavData.IsValid())
	{
		QueryFilter = NavData->GetDefaultQueryFilter();
	}
}

bool UVectorFieldNavigationSystem::RegisterNavData(ANavigationData* InNavData)
{
	FScopeLock Lock(&NavDataRegistration);
	const bool retval = NavDataSet.AddUnique(InNavData) > INDEX_NONE;
	InNavData->OnRegistered();
	return retval;
}

void UVectorFieldNavigationSystem::UnRegisterNavData(ANavigationData* InNavData)
{
}

FPathFindingResult UVectorFieldNavigationSystem::FindVectorFieldPathSync(FVectorFieldQuery& InQuery)
{
	FPathFindingResult retval(ENavigationQueryResult::Error);
	if (InQuery.NavData.IsValid() == false)
	{

	}
	else
	{
		retval = InQuery.NavData->FindPath(InQuery.NavAgentProperties, InQuery);
	}


	return retval;
}

void UVectorFieldNavigationSystem::Tick(float DeltaSeconds)
{
	int debug = 9;
}

void UVectorFieldNavigationSystem::CleanUp(const FNavigationSystem::ECleanupMode Mode)
{
}

void UVectorFieldNavigationSystem::Configure(const UNavigationSystemConfig& Config)
{
}

void UVectorFieldNavigationSystem::AppendConfig(const UNavigationSystemConfig& NewConfig)
{
}

void UVectorFieldNavigationSystem::OnInitializeActors()
{
}

bool UVectorFieldNavigationSystem::IsNavigationBuilt(const AWorldSettings* Settings) const
{
	return true;
}

void UVectorFieldNavigationSystem::ApplyWorldOffset(const FVector& InOffset, bool bWorldShift)
{

}

void UVectorFieldNavigationSystem::InitializeForWorld(UWorld& InWorld, FNavigationSystemRunMode Mode)
{
	for (TActorIterator<ANavigationData> It(&InWorld); It; ++It)
	{
		ANavigationData* NavData = (*It);
		if (NavData != NULL)
		{
			const bool Result = RegisterNavData(NavData);
			if (Result == true)
			{
				// allowing full rebuild of the entire navmesh only for the fully dynamic generation modes
				// other modes partly rely on the serialized data and full rebuild would wipe it out
				NavData->RebuildAll();
			}
			else
			{
				NavData->CleanUpAndMarkPendingKill();
			}
		}
	}
}

void UVectorFieldNavigationSystem::RequestRegistrationDeferred(ANavigationData& NavData)
{
	FScopeLock RegistrationLock(&NavDataRegistrationSection);

	if (NavDataRegistrationQueue.Num() < 10)
	{
		NavDataRegistrationQueue.AddUnique(&NavData);
	}
}

ANavigationData* UVectorFieldNavigationSystem::GetNavDataForProps(const FNavAgentProperties& AgentProperties, const FVector& AgentLocation, const FVector& Extent) const
{

	UE_CLOG(!AgentProperties.IsValid(), LogNavigation, Warning, TEXT("Looking for NavData using invalid FNavAgentProperties."));

	ANavigationData* retval = nullptr;

	if (NavDataSet.Num() == 1U)
	{
		retval = NavDataSet[0];
	}

	/*
	const TWeakObjectPtr<ANavigationData>* NavDataForAgent = AgentToNavDataMap.Find(AgentProperties);
	ANavigationData* NavDataInstance = NavDataForAgent ? NavDataForAgent->Get() : nullptr;

	if (NavDataInstance == nullptr)
	{
		TArray<FNavAgentProperties> AgentPropertiesList;
		AgentToNavDataMap.GenerateKeyArray(AgentPropertiesList);

		FNavAgentProperties BestFitNavAgent;
		float BestExcessHeight = -FLT_MAX;
		float BestExcessRadius = -FLT_MAX;
		float ExcessRadius = -FLT_MAX;
		float ExcessHeight = -FLT_MAX;
		const float AgentHeight = AgentProperties.AgentHeight;

		for (TArray<FNavAgentProperties>::TConstIterator It(AgentPropertiesList); It; ++It)
		{
			const FNavAgentProperties& NavIt = *It;
			const bool bNavClassMatch = NavIt.IsNavDataMatching(AgentProperties);
			if (!bNavClassMatch)
			{
				continue;
			}

			ExcessRadius = NavIt.AgentRadius - AgentProperties.AgentRadius;
			ExcessHeight =  (NavIt.AgentHeight - AgentHeight);

			const bool bExcessRadiusIsBetter = ((ExcessRadius == 0) && (BestExcessRadius != 0))
				|| ((ExcessRadius > 0) && (BestExcessRadius < 0))
				|| ((ExcessRadius > 0) && (BestExcessRadius > 0) && (ExcessRadius < BestExcessRadius))
				|| ((ExcessRadius < 0) && (BestExcessRadius < 0) && (ExcessRadius > BestExcessRadius));
			const bool bExcessHeightIsBetter = ((ExcessHeight == 0) && (BestExcessHeight != 0))
				|| ((ExcessHeight > 0) && (BestExcessHeight < 0))
				|| ((ExcessHeight > 0) && (BestExcessHeight > 0) && (ExcessHeight < BestExcessHeight))
				|| ((ExcessHeight < 0) && (BestExcessHeight < 0) && (ExcessHeight > BestExcessHeight));
			const bool bBestIsValid = (BestExcessRadius >= 0) && (BestExcessHeight >= 0);
			const bool bRadiusEquals = (ExcessRadius == BestExcessRadius);
			const bool bHeightEquals = (ExcessHeight == BestExcessHeight);

			bool bValuesAreBest = ((bExcessRadiusIsBetter || bRadiusEquals) && (bExcessHeightIsBetter || bHeightEquals));
			if (!bValuesAreBest && !bBestIsValid)
			{
				bValuesAreBest = bExcessRadiusIsBetter || (bRadiusEquals && bExcessHeightIsBetter);
			}

			if (bValuesAreBest)
			{
				BestFitNavAgent = NavIt;
				BestExcessHeight = ExcessHeight;
				BestExcessRadius = ExcessRadius;
			}
		}

		if (BestFitNavAgent.IsValid())
		{
			NavDataForAgent = AgentToNavDataMap.Find(BestFitNavAgent);
			NavDataInstance = NavDataForAgent ? NavDataForAgent->Get() : nullptr;
		}
	}
	*/
	return retval;
}
