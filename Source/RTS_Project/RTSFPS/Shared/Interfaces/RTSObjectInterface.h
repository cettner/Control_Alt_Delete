// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../../RTS/Orders/RTSOrder.h"
#include "../../RTS/UI/Properties/RTSProperty.h"
#include "RTSObjectInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class URTSObjectInterface : public UInterface
{
	GENERATED_BODY()
};


class RTS_PROJECT_API IRTSObjectInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void SetSelected();
	virtual void SetDeselected();
	virtual bool IsBoxSelectable() const;

	virtual int GetTeam() const;
	virtual void SetTeam(int teamid);
	virtual void SetTeamColors(FLinearColor TeamColor);
	/*Called when the local players team has been set either from initializtion, replication, or runtime*/
	virtual void OnLocalPlayerTeamChange(int InLocalTeamID);

	virtual bool IsAlive() const;
	static bool IsRTSObjectValid(const IRTSObjectInterface * InRTSObject);

	virtual void SetTarget(AActor* InNewTarget);
	virtual void SetTargetLocation(FVector InTargetLocation);
	virtual AActor* GetTarget();
	virtual void ClearTarget();

	virtual bool IsOrderablebyController(const AController * InController) const;
	virtual bool CanCompleteOrder(const TSubclassOf<URTSOrder> InOrderClass) const;
	virtual const TArray<TSubclassOf<URTSOrder>> GetAvailableOrders() const;
	virtual TSubclassOf<URTSOrder> GetDefaultOrder(const FHitResult& InHitContext) const;
	virtual void IssueOrder(const FHitResult& InHitContext, const TSubclassOf<URTSOrder> InOrderClass = nullptr, const bool InbIsQueuedOrder = false);
	virtual TSubclassOf<URTSOrder> GetCurrentOrder() const;
	virtual TArray<TSubclassOf<URTSOrder>> GetAllIssuedOrders() const;

	virtual TArray<TSubclassOf<URTSProperty>> GetRTSProperties() const;
	virtual bool ContainsProperty(TSubclassOf<URTSProperty> InPropCheck) const;

	virtual IRTSObjectInterface* GetLeadRTSObject();

	template<class T>
	T* GetLeadRTSObject() { return Cast<T>(GetLeadRTSObject()); }

protected:
	virtual void RegisterRTSObject() = 0;
	virtual void UnRegisterRTSObject() = 0;
};
