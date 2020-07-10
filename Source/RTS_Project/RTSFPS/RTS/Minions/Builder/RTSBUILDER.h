// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../../BaseClasses/RTSMinion.h"
#include "RTSBUILDER.generated.h"

/**
 * 
 */


class AResource; class ARTSStructure;

enum Builder_State
{
	IDLE,
	MINE_ON_ROUTE,
	MINING,
	DELIVERY_ON_ROUTE,
	DElIVERY,
};

UCLASS()
class RTS_PROJECT_API ARTSBUILDER : public ARTSMinion
{
	GENERATED_BODY()

public:
	ARTSBUILDER();
	virtual bool HasAssets() override;
	virtual void ReleaseAssets() override;
	virtual bool CanInteract(AActor * Interactable) override;

	bool CanCarryMore(); 
	bool CanMine();

	ARTSStructure * Get_Nearest_Dropoint();
	
	void StartMining(AResource * Node);


	int carried_resource = 0;
	int max_resource = 50;
	

protected:

	Builder_State state = IDLE;
	int mine_amount = 5;
	float mine_interval = 1.0;



private:
	UPROPERTY(Transient)
	AResource * target_node;

	FVector Node_Local;

	ARTSStructure * target_struct;
	FTimerHandle Mine_Handler;

	UPROPERTY(EditDefaultsOnly)
	float mine_range = 200.0;
	UPROPERTY (EditDefaultsOnly)
	float deliver_range = 500.0;

	bool node_timer_set = false;
	bool is_state_machine_active = false;
	int node_ref = -1;
	TArray<int> type_count;

	void Mine_Resource();
	void DeliverResources();
	bool Drop_Point_Available();
	bool Node_Nearby(FVector check_local);
};
