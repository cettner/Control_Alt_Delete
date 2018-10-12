// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTSMinion.h"
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
	void Set_Node(AResource * current_node);
	void Set_Structure(ARTSStructure * current_struct);
	virtual void Tick(float DeltaSeconds) override;

	virtual bool HasAssets() override;
	virtual void ReleaseAssets(FVector Base_Order) override;

	void Check_Mine_Status();
	void Mine_Resource();

	void Check_Delivery_Status();

	TArray<int> type_count;
	int carried_resource = 0;
	int max_resource = 50;
	

protected:

	Builder_State state = IDLE;
	int mine_amount = 5;
	float mine_interval = 1.0;



private:
	AResource * target_node;
	ARTSStructure * target_struct;
	FTimerHandle Mine_Handler;
	float mine_range = 175.0;
	bool node_timer_set = false;
	bool is_state_machine_active = false;
	int node_ref = -1;

	void DeliverResources();
	bool Drop_Point_Available();
};
