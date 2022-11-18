// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../RTSMinion.h"
#include "RTSBUILDER.generated.h"

/**
 * 
 */


class AResource; class ARTSStructure;

UCLASS()
class RTS_PROJECT_API ARTSBUILDER : public ARTSMinion
{
	GENERATED_BODY()

public:
	ARTSBUILDER();
//	virtual bool HasAssets() override;
//	virtual void ReleaseAssets() override;
	virtual bool CanInteract(AActor * Interactable) override;

public:

	bool CanCarryMore(); 
	virtual bool DeliverResources(ARTSStructure* Structure);
	
	UFUNCTION(BlueprintCallable)
	bool IsMining();
	
	void StartMining(AResource * Node);
	virtual int GetCurrentWeight() const;
	virtual int GetMaxWeight() const;
	virtual int GetWeightof(TSubclassOf<AResource> ResourceType) const;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	TMap<TSubclassOf<AResource>, int> CarriedResources;

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

	void MineResource();
	void AddResource(TSubclassOf<AResource> type, int amount);
	void CalculateCurrentWeight();
	int CalculateGatherAmount(TSubclassOf<AResource> type) const;
};
