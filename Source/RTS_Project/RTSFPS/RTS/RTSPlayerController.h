// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTSHUD.h"
#include "RTSMinion.h"
#include "RTS_Project/RTSFPS/RTS/Camera/RTSCamera.h"
#include "RTS_Project/RTSFPS/RTS/FOW/FogOfWarManager.h"
#include "RTS_Project/TeamMultiplayerGame/Game/GameArchitecture/DefaultPlayerController.h"
#include "Orders/RTSTargetedOrder.h"
#include "RTSPlayerController.generated.h"



/**
 * 
 */


UCLASS()
class RTS_PROJECT_API ARTSPlayerController : public ADefaultPlayerController
{
	GENERATED_BODY()
	
public:
	virtual void IssueOrder(const TArray<TScriptInterface<IRTSObjectInterface>>& InUnits, const URTSOrder* InOrder, const FHitResult InHitContext = FHitResult(), const bool InbIsQueuedOrder = false);

	virtual void IssueDefaultOrder(const TArray<TScriptInterface<IRTSObjectInterface>>& InUnits, const FHitResult InHitContext = FHitResult(), const bool InbIsQueuedOrder = false);

protected:
	virtual const URTSTargetedOrder* CreateTargetOrder(const TSubclassOf<URTSTargetedOrder> OrderClass, const FHitResult& InHitContext = FHitResult());

protected:
	ARTSPlayerController();

	UFUNCTION(Server, reliable, WithValidation)
	void ServerIssueOrder(const TArray<UObject*>& InUnits, const URTSOrder* InOrder, const FHitResult InHitContext = FHitResult(), const bool InbIsQueuedOrder = false);

	UFUNCTION(Server, reliable, WithValidation)
	void ServerIssueDefaultOrder(const TArray<UObject*>& InUnits, const FHitResult InHitContext = FHitResult(), const bool InbIsQueuedOrder = false);

	virtual void SetupInputComponent() override;

	virtual void SetPawn(APawn* InPawn) override;


public:
	virtual void FinishLocalPlayerSetup() override;

public:
	virtual TArray<IRTSObjectInterface*> GetOrderableUnits(TSubclassOf<AActor> InObjectClass = AActor::StaticClass()) const;

	template<typename ClassFilter>
	TArray<IRTSObjectInterface*> GetOrderableUnits();

	virtual bool IsUnitOrderable(const IRTSObjectInterface * InObj) const;


};


