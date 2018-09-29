// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTSMinion.h"
#include "RTSBUILDER.generated.h"

/**
 * 
 */

class AResource;

enum Builder_State
{
	IDLE,
	MINE_ON_ROUTE,
	MINING,
	DElIVERY,
};

UCLASS()
class RTS_PROJECT_API ARTSBUILDER : public ARTSMinion
{
	GENERATED_BODY()

public:
	void Set_Node(AResource * current_node);
	virtual void Tick(float DeltaSeconds) override;

	void Check_Mine_Status();
	void Mine_Resource();


	int carried_resource = 0;
	int max_resource = 50;
	

protected:

	Builder_State state = IDLE;
	int mine_amount = 5;
	float mine_interval = 1.0;



private:
	AResource * target_node;
	FTimerHandle Mine_Handler;
	float mine_range = 175.0;
	

};
