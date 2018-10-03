// Fill out your copyright notice in the Description page of Project Settings.

#include "RTSBUILDER.h"
#include "Resource.h"
#include "Runtime/Engine/Public/TimerManager.h "
#include "Engine.h"
#include "Runtime/Engine/Classes/AI/Navigation/NavigationSystem.h"




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

bool ARTSBUILDER::HasAssets()
{
	if (bismovespecial  || (state != IDLE))  // the unit is doing something so handle the request in ReleaseAssets();
	{
		return (true);
	}
	return(false);
}

void ARTSBUILDER::ReleaseAssets(FVector Base_Order)  // This function handles interupts made by the user while they are currently enacting a state.
{
	if (bismovespecial)  // we recieved orders from elsewhere so ignore the call the first time
	{
		bismovespecial = false;
	}
	else if(state == MINING || state == MINE_ON_ROUTE)
	{
		if (IsValid(target_node))
		{
			target_node->FreeSlot(node_ref);
		}
		target_node = NULL;
		node_ref = -1;
		state = IDLE;
		UNavigationSystem::SimpleMoveToLocation(this->GetController(), Base_Order);
	}
	else
	{
		state = IDLE;
		UNavigationSystem::SimpleMoveToLocation(this->GetController(),Base_Order);
	}
}

void ARTSBUILDER::Set_Node(AResource * current_node)   //EDIT THIS
{
	
		if (IsValid(current_node)  && current_node != target_node)
		{
			target_node = current_node;
			state = MINE_ON_ROUTE;
			FVector nodelocal = target_node->GetSlot(node_ref);
			bismovespecial = true;

			UNavigationSystem::SimpleMoveToLocation(this->GetController(), nodelocal);
		}
		else if (current_node == target_node && (state == MINING || state == MINE_ON_ROUTE)) // we're already mining or traveling to this node, so ignore the command.
		{
			bismovespecial = true;
		}
		else
		{
			state = IDLE;
		}
	}


void ARTSBUILDER::Check_Mine_Status()
{
	if (IsValid(target_node))
	{
		FVector mylocal = GetActorLocation();
		FVector nodelocal = target_node->GetActorLocation();

		float distance = mylocal.Dist(mylocal, nodelocal);
		if (carried_resource == max_resource)
		{
			if (node_ref != -1)
			{
				target_node->FreeSlot(node_ref);
			}
			state = DElIVERY;
		}
		else if (distance < mine_range && !node_timer_set) // we're in range of the node and we havnt set the timer already
		{
			state = MINING;

			FRotator newrot = (nodelocal - mylocal).Rotation();
			newrot.Pitch = 0;
			RootComponent->SetWorldRotation(newrot);

			GetWorldTimerManager().SetTimer(Mine_Handler, this, &ARTSBUILDER::Mine_Resource, 1.0, false, mine_interval);
			node_timer_set = true;
		}
		else if (distance < mine_range && state == MINE_ON_ROUTE)
		{
			state = MINING;
		}
	}
	else
	{
		node_ref = -1;
	}
}

void ARTSBUILDER::Mine_Resource()
{
	node_timer_set = false;
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
