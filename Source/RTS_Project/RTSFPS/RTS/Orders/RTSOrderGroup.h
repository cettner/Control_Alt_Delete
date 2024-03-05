// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "EnvironmentQuery/EnvQueryTypes.h"

#include "RTSTargetedOrder.h"
#include "RTSOrderGroup.generated.h"


USTRUCT()
struct FOrderContext
{
	GENERATED_USTRUCT_BODY()

	friend class URTSOrderGroup;

	virtual FVector GetContextPoint() const;

	virtual IRTSObjectInterface* GetRTSContext() const
	{
		return ContextActor;
	}

protected:
	IRTSObjectInterface* ContextActor = nullptr;

	FVector OriginalContextHitPoint = FVector();

	FVector LastValidOrderPoint = FVector();

	bool bIsValid = false;

	FORCEINLINE bool IsValid() const { return bIsValid; }
};


UCLASS()
class RTS_PROJECT_API URTSOrderGroup : public UObject
{
	GENERATED_BODY()

	friend class ARTSOrderManager;

public:
	FORCEINLINE bool HasBeenIssued() const { return bIsIssued; }
	FORCEINLINE uint32 GetOrderID() const { return OrderID; }
	FORCEINLINE const FOrderContext& GetOrderContext() const { return OrderContext; }
	FORCEINLINE AController* GetOrderIssuer() const { return OrderIssuer; }
	TArray<TScriptInterface<IRTSObjectInterface>> GetAllActiveUnits() const;

protected:
	const TSet<URTSOrder*>& GetOrders() const;
	bool InitalizeOrderGroup(const uint32 InID, const TArray<TScriptInterface<IRTSObjectInterface>>& InUnits, AController* InIssuer, const FHitResult& InHitContext = FHitResult(), const bool InbIsQueuedOrder = false);
	virtual FOrderContext ReadyOrderContext(const FHitResult& InHitContext);
	bool IssueAllOrders();

public:
	virtual bool AreQueryResultsAvailable(const TObjectPtr<UEnvQuery>& InTemplate) const;
	/*Returns the signature of the pending query if available, will return null even if results are available*/
	virtual FQueryFinishedSignature* IsQueryPending(const TObjectPtr<UEnvQuery>& InTemplate);
	virtual FQueryFinishedSignature* RequestStartQuery(const TObjectPtr<UEnvQuery>& InTemplate, URTSOrder * RequestingOrder);
	virtual const TSharedPtr<FEnvQueryResult> GetQueryResults(const TObjectPtr<UEnvQuery>& InTemplate) const;
	void AddOrderQueryResult(const TObjectPtr<UEnvQuery>& InQueryClass, const TSharedPtr<FEnvQueryResult>& InResult);

protected:
	virtual URTSTargetedOrder* CreateTargetOrder(const TSubclassOf<URTSTargetedOrder>& OrderClass);
	virtual void OnOrderAbandoned(URTSOrder* AbandonedOrder);

protected:
	AController* OrderIssuer = nullptr;

	uint32 OrderID = INVALID_ORDER_ID;
	
	UPROPERTY()
	TSet<URTSOrder *> OrderMapping = TSet<URTSOrder*>();

	TMap<TObjectPtr<UEnvQuery>, FQueryFinishedSignature> PendingQueries = TMap<TObjectPtr<UEnvQuery>, FQueryFinishedSignature>();
	TMap<TObjectPtr<UEnvQuery>, TSharedPtr<FEnvQueryResult>> QueryMapping = TMap<TObjectPtr<UEnvQuery>, TSharedPtr<FEnvQueryResult>>();

	FOrderContext OrderContext = FOrderContext();

	bool bIsIssued = false;
};

