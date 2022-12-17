// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../RTSMinion.h"
#include "../../../GameSystems/ResourceSystem/Interfaces/ResourceGatherer.h"
#include "../../Orders/RTSMineResourceOrder.h"
#include "RTSBUILDER.generated.h"

/**
 * 
 */


class AResource; class ARTSStructure;

UCLASS()
class RTS_PROJECT_API ARTSBUILDER : public ARTSMinion, public IResourceGatherer
{
	GENERATED_BODY()

public:
	virtual bool DeliverResources(ARTSStructure* Structure);
	
	UFUNCTION(BlueprintCallable)
	bool IsMining() const;
	void StartMining(AResource* Node);
	bool StopMining();

	void MineResource(); 
	bool ExtractResource(AResource* Node);

	virtual void AddResource(TSubclassOf<AResource> type, int amount) override;
	virtual bool RemoveResource(const TSubclassOf<AResource> ResourceClass, int amount) override;
	virtual uint32 GetHeldResource(TSubclassOf<AResource> ResourceClass) const override;
	virtual uint32 GetCurrentWeight() const override;
	virtual uint32 GetMaxWeight() const override;

protected:
	void SetIsMining(const bool InMiningState);
	virtual AResource* GetTargetResource() const;

protected:
	virtual const TSubclassOf<URTSTargetedOrder> GetDefaultOrderClass(const FHitResult& InHitContext) const override;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = Orders)
	const TSubclassOf<URTSMineResourceOrder> MineOrderClass = URTSMineResourceOrder::StaticClass();

	UPROPERTY(EditDefaultsOnly, Category = Resources)
	uint32 MaxCarryWeight = 50;

	UPROPERTY(EditDefaultsOnly, Category = Resources)
	uint32 MineAmount = 5;

	UPROPERTY(EditDefaultsOnly, Category = Resources)
	float MineInterval = 1.0;

protected:
	TMap<TSubclassOf<AResource>, uint32> CarriedResources;

	int CurrentWeight = 0;

protected:
private:
	FTimerHandle MineHandler;

	UPROPERTY(Replicated)
	bool bIsMining = false;
};
