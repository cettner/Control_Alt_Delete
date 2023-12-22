// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "RTSTargetedOrder.h"
#include "RTSOrderGroup.generated.h"




UCLASS()
class RTS_PROJECT_API URTSOrderGroup : public UObject
{
	GENERATED_BODY()

public:
	bool MatchingContext(const FHitResult& InHitContext, const AController* InController) const;
	FORCEINLINE bool HasBeenIssued() const { return bIsIssued; }
	FORCEINLINE uint32 GetOrderID() const { return OrderID; }
	TArray<TScriptInterface<IRTSObjectInterface>> GetAllActiveUnits() const;

	const TArray<URTSOrder *> GetOrders() const;
	bool InitalizeOrderGroup(const uint32 InID, const TArray<TScriptInterface<IRTSObjectInterface>>& InUnits, AController* InIssuer, const FHitResult& InHitContext = FHitResult(), const bool InbIsQueuedOrder = false);
	bool IssueOrder();
	bool DeregisterUnit(const TScriptInterface<IRTSObjectInterface> &InUnit, URTSOrder * InKey);


protected:
	virtual URTSTargetedOrder* CreateTargetOrder(const TSubclassOf<URTSTargetedOrder>& OrderClass, const FHitResult& InHitContext = FHitResult());

protected:
	virtual void BeginDestroy() override;

protected:
	AController* OrderIssuer = nullptr;

	uint32 OrderID = INVALID_ORDER_ID;

	FHitResult OriginalContext = FHitResult();
	TMultiMap<URTSOrder *, TScriptInterface<IRTSObjectInterface>> OrderMapping = TMultiMap<URTSOrder *, TScriptInterface<IRTSObjectInterface>>();

	bool bIsIssued = false;
};
