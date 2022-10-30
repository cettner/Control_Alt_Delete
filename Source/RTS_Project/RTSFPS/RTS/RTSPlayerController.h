// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTSHUD.h"
#include "RTSMinion.h"
#include "RTS_Project/RTSFPS/RTS/Camera/RTSCamera.h"
#include "RTS_Project/RTSFPS/RTS/FOW/FogOfWarManager.h"
#include "RTS_Project/TeamMultiplayerGame/Game/GameArchitecture/DefaultPlayerController.h"
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

public:
	ARTSPlayerController();

	UFUNCTION(Server, reliable, WithValidation)
	void MoveMinions(ARTSPlayerController * PC, const TArray<ARTSMinion *>& Units, FHitResult Hit);

	UFUNCTION(Server, reliable, WithValidation)
	void ServerPurchaseRTSObject(ARTSStructure * SpawningStructure, TSubclassOf<UObject> RequestedClass);

	UFUNCTION(Server, reliable, WithValidation)
	void ServerPurchaseStructure(TSubclassOf<AActor> RequestedClass, FTransform BuildLocation);

	UFUNCTION(Server, reliable, WithValidation)
	void ServerIssueOrder(const TArray<TScriptInterface<IRTSObjectInterface>>& InUnits, const URTSOrder* InOrder, const FHitResult InHitContext = FHitResult(), const bool InbIsQueuedOrder = false);

	virtual void SetupInputComponent() override;

	virtual void SetPawn(APawn* InPawn) override;

	virtual void FinishLocalPlayerSetup() override;
	
	virtual AFogOfWarManager * InitFOW();

	virtual TArray<IRTSObjectInterface*> GetOrderableUnits(TSubclassOf<AActor> InObjectClass = AActor::StaticClass()) const;

	template<typename ClassFilter>
	TArray<IRTSObjectInterface*> GetOrderableUnits();

	virtual bool IsUnitOrderable(const IRTSObjectInterface * InObj) const;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Fog Of War")
	TSubclassOf<AFogOfWarManager> FOWManagerClass;
	AFogOfWarManager * FOWManager;

};


