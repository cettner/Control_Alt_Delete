// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../RTSMinion.h"
#include "../../../GameSystems/ResourceSystem/Interfaces/ResourceGatherer.h"
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
	ARTSBUILDER();
	virtual bool CanInteract(AActor * Interactable) override;

public:
	virtual bool DeliverResources(ARTSStructure* Structure);
	
	UFUNCTION(BlueprintCallable)
	bool IsMining() const;
	void StartMining(AResource* Node);
	void MineResource();

	virtual void AddResource(TSubclassOf<AResource> type, int amount) override;
	virtual bool RemoveResource(const TSubclassOf<AResource> ResourceClass, int amount) override;
	virtual uint32 GetHeldResource(TSubclassOf<AResource> ResourceClass) const override;
	virtual uint32 GetCurrentWeight() const override;
	virtual uint32 GetMaxWeight() const override;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	TMap<TSubclassOf<AResource>, uint32> CarriedResources;

	UPROPERTY(EditDefaultsOnly)
	int MaxCarryWeight = 50;

	int CurrentWeight = 0;

protected:
	int MineAmount = 5;
	float MineInterval = 1.0;

private:
	UPROPERTY(Transient)
	AResource * target_node;

	FTimerHandle MineHandler;

	UPROPERTY(Replicated)
	bool bIsMining = false;

	int CalculateGatherAmount(TSubclassOf<AResource> type) const;
};
