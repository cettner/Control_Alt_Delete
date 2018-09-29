// Fill out your copyright notice in the Description page of Project Settings.

#include "RTSBUILDER.h"
#include "Resource.h"
#include "Runtime/Engine/Public/TimerManager.h "
#include "Engine.h"

void ARTSBUILDER::Set_Node(AResource * current_node)
{
	if (current_node != target_node)
	{
		target_node = current_node;
		state = MINE_ON_ROUTE;
	}

}


// Called every frame
void ARTSBUILDER::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (state)
	{
	case MINE_ON_ROUTE:
		Check_Mine_Status();
		break;

	default: // IDLE
		break;
	}

}

void ARTSBUILDER::Check_Mine_Status()
{
	FVector mylocal = GetActorLocation();

	float distance = mylocal.Dist(mylocal, target_node->GetActorLocation());
	if (carried_resource == max_resource)
	{
		state = DElIVERY;
	}
	else if (distance <  mine_range ) // we're in range of the node and can carry more
	{
		state = MINING;
		GetWorldTimerManager().SetTimer(Mine_Handler, this, &ARTSBUILDER::Mine_Resource, 1.0, false, mine_interval);
	}

}

void ARTSBUILDER::Mine_Resource()
{
	if (IsValid(target_node) && state == MINING)
	{
		int gather_amount = (max_resource - carried_resource);  // determine how much room we have to add.
		int added_resource = 0;

		if (gather_amount < mine_amount)  //ask the node for less if we can only fit that much
		{
			added_resource = target_node->Mine(gather_amount);
		}
		else    // we can ask for the full value
		{
			gather_amount = mine_amount; 
			added_resource = target_node->Mine(gather_amount);
		}

		if (added_resource == gather_amount)  // we got the amount we requested so ask for more.
		{
			carried_resource += added_resource;
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Carrying %d of %d"),carried_resource, max_resource));
			Check_Mine_Status();
		}
		else  // the node was destroyed by our request.
		{
			carried_resource += added_resource;
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Carrying %d of %d"), carried_resource, max_resource));
		}
	}

}
