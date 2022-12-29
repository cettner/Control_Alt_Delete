// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../../RTS/Orders/RTSTargetedOrder.h"
#include "../../RTS/UI/Properties/RTSPropertyContainer.h"
#include "RTSObjectInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class URTSObjectInterface : public UInterface
{
	GENERATED_BODY()
};

constexpr int NEUTRAL_TEAM_INDEX = -1;

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
	virtual bool IsLocalEnemy() const;

	virtual bool IsAlive() const;
	static bool IsRTSObjectValid(const IRTSObjectInterface * InRTSObject);

	virtual bool IsOrderablebyController(const AController * InController) const;
	virtual bool CanCompleteOrder(const URTSOrder* InOrder) const;
	virtual const TArray<const URTSOrder*> GetAvailableOrders() const;
	virtual const TSubclassOf<URTSTargetedOrder> GetDefaultOrderClass(const FHitResult& InHitContext) const;
	
	virtual void IssueOrder(AController* Issuer, const FHitResult& InHitContext, URTSOrder* InOrderClass = nullptr, const bool InbIsQueuedOrder = false);

	virtual const URTSOrder* GetCurrentOrder() const;
	virtual TArray<const URTSOrder*> GetAllIssuedOrders() const;

	virtual TArray<URTSProperty*> GetRTSProperties(bool bIncludeNestedProperties = false) const;
	virtual bool ContainsProperty(const URTSProperty * InPropCheck) const;

	virtual IRTSObjectInterface* GetLeadRTSObject();

	template<class T>
	T* GetLeadRTSObject() { return Cast<T>(GetLeadRTSObject()); }

public:
	virtual UTexture* GetThumbnail(const UUserWidget * InDisplayContext = nullptr) const;
	virtual FName GetUnitName() const;

protected:
	virtual void RegisterRTSObject() = 0;
	virtual void UnRegisterRTSObject() = 0;
};
