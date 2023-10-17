// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "../../RTSMinion.h"
#include "../../../GameSystems/ResourceSystem/Interfaces/ResourceGatherer.h"
#include "../../Orders/RTSMineResourceOrder.h"
#include "../../../GameSystems/ResourceSystem/ResourceGathererComponent.h"
#include "RTSBUILDER.generated.h"

/**
 * 
 */


class ARTSStructure;

UCLASS()
class RTS_PROJECT_API ARTSBUILDER : public ARTSMinion
{
	GENERATED_BODY()

	ARTSBUILDER();

public:
	virtual bool DeliverResources(ARTSStructure* Structure);
	
	UFUNCTION(BlueprintCallable)
	bool IsMining() const;
	void StartMining(AMineableResource* Node);
	bool StopMining();

	void MineResource(); 
	bool ExtractResource(AMineableResource* Node);
	UFUNCTION()
	virtual void OnResourceNodeDepleted();

protected:
	void SetIsMining(const bool InMiningState);
	virtual AMineableResource* GetTargetResource() const;

protected:
	virtual const TSubclassOf<URTSTargetedOrder> GetDefaultOrderClass(const FHitResult& InHitContext) const override;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = Orders)
	const TSubclassOf<URTSMineResourceOrder> MineOrderClass = URTSMineResourceOrder::StaticClass();

	UPROPERTY(EditDefaultsOnly, Category = Resources)
	uint32 MineAmount = 5;

	UPROPERTY(EditDefaultsOnly, Category = Resources)
	float MineInterval = 1.0;

protected:
	int CurrentWeight = 0;

	FTimerHandle MineHandler;

	FDelegateHandle NodeDestroyedHandle;

	UPROPERTY(Replicated)
	bool bIsMining = false;
};
