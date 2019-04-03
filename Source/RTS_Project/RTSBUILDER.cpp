// Fill out your copyright notice in the Description page of Project Settings.

#include "RTSBUILDER.h"
#include "Resource.h"
#include "RTSStructure.h"
#include "GameFramework/PlayerController.h"
#include "RTSPlayerController.h"
#include "Runtime/Engine/Public/TimerManager.h "
#include "GameFramework/Actor.h"
#include "Engine.h"


ARTSBUILDER::ARTSBUILDER()
{
	for (int i = 0; i < NUM_RESOURCES; i++)
	{
		type_count.Add(0);
	}
}

void ARTSBUILDER::Set_Structure(ARTSStructure * current_struct)
{
	if (IsValid(current_struct) && current_struct != target_struct)
	{
		target_struct = current_struct;

		if (carried_resource > 0)
		{
			state = DELIVERY_ON_ROUTE;
			bismovespecial = true;
			RtsMoveToActor(target_struct);
		}
		else if (current_struct == target_struct && state == DELIVERY_ON_ROUTE)
		{
			bismovespecial = true;
		}
		else
		{
			state = IDLE;
		}
	}
}

void ARTSBUILDER::Check_Delivery_Status()
{
	if (IsValid(target_struct))
	{
		FVector mylocal = GetActorLocation();
		FVector structlocal = target_struct->GetActorLocation();
		float distance = mylocal.Dist(mylocal, structlocal);

		if (distance < deliver_range)
		{
			DeliverResources();

			if (is_state_machine_active)  // automatically go back to the node we were mining, or a node nearby.
			{
				if (IsValid(target_node)) // the node we were mining is still legit.
				{
					state = MINE_ON_ROUTE;
					FVector nodelocal = target_node->GetSlot(node_ref);
					RtsMove(nodelocal);
				}
				else if (false) // node we were mining is full or gone so look to see if a nearby one exists.
				{
					// do stuff here for NodeLocal(); 
				}
				else // we couldnt find anything to mine within reason.
				{
					is_state_machine_active = false;
					state = IDLE;
				}
				
			}
			else
			{
				state = IDLE;
			}

		}

	}
	else // Structure was destroyed before we delivered.
	{
		target_struct = NULL;
	}
}

ARTSStructure * ARTSBUILDER::Get_Nearest_Dropoint()
{

	float closest_dist = FLT_MAX;
	float dist;

	FVector mylocal = GetActorLocation();

	ARTSStructure * retval = NULL;

	for (TActorIterator<ARTSStructure>ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if (ActorItr->IsDropPoint())
		{
			FVector struct_local = ActorItr->GetActorLocation();
			dist = mylocal.Dist(mylocal, struct_local);

			if (dist < closest_dist)
			{
				retval = *ActorItr;
				closest_dist = dist;
			}

		}
	}

	return retval;
}

void ARTSBUILDER::DeliverResources()
{
	ARTSPlayerController * PC = Cast<ARTSPlayerController>(GetWorld()->GetFirstPlayerController());

	for (int i = 0; i < NUM_RESOURCES; i++)
	{
		if (type_count[i] > 0)
		{
			PC->AddResource(type_count[i], (Resource_Types)i);
			type_count[i] = 0;
		}
	}

	carried_resource = 0;
}

bool ARTSBUILDER::Drop_Point_Available()
{
	return (true);
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
	case MINING:
		Check_Node_Status();
		break;
	case DELIVERY_ON_ROUTE:
		Check_Delivery_Status();
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


void ARTSBUILDER::ReleaseAssets()
{
	if (bismovespecial)  // we recieved orders from elsewhere so ignore the call the first time
	{
		bismovespecial = false;
	}
	else if (state == MINING || state == MINE_ON_ROUTE)
	{
		if (IsValid(target_node))
		{
			target_node->FreeSlot(node_ref);
		}
		target_node = NULL;
		target_struct = NULL;
		node_ref = -1;
		state = IDLE;
		is_state_machine_active = false;
	}
	else if (state == DELIVERY_ON_ROUTE)
	{
		target_struct = NULL;
		target_node = NULL;
		state = IDLE;
		is_state_machine_active = false;
	}
	else
	{
		state = IDLE;
		is_state_machine_active = false;
	}
}

bool ARTSBUILDER::CanInteract(AActor * Interactable)
{
	if(Cast<AResource>(Interactable))
	{
		return(true);
	}
	else
	{
		return(false);
	}
}

void ARTSBUILDER::Set_Node(AResource * current_node)   // IsValid in the past has thrown an unrecognizable exception....
{
	
		if (IsValid(current_node)  && current_node != target_node)
		{
			target_node = current_node;
			state = MINE_ON_ROUTE;
			FVector nodelocal = target_node->GetSlot(node_ref);
			bismovespecial = true;

			RtsMove(nodelocal);
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

			ARTSStructure * droppoint = Get_Nearest_Dropoint();
			if(droppoint != NULL)
			{		
				is_state_machine_active = true;
				target_struct = droppoint;
				state = DELIVERY_ON_ROUTE;
				RtsMoveToActor(droppoint);
			}
			else
			{
				state = IDLE;
			}

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
		ARTSStructure * droppoint = Get_Nearest_Dropoint();
		if (droppoint != NULL)
		{
			is_state_machine_active = true;
			target_struct = droppoint;
			state = DELIVERY_ON_ROUTE;
			RtsMoveToActor(droppoint);
		}
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
		Resource_Types type = NULL_TYPE;

		if (gather_amount < mine_amount)  //ask the node for less if we can only fit that much
		{
			
			added_resource = target_node->Mine(gather_amount,type);
		}
		else    // we can ask for the full value
		{
			gather_amount = mine_amount; 
			added_resource = target_node->Mine(gather_amount,type);
		}

		if (added_resource == gather_amount)  // we got the amount we requested so ask for more.
		{
			carried_resource += added_resource;
			type_count[type] += added_resource;
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Carrying %d of %d"),carried_resource, max_resource));
			Check_Mine_Status();
		}
		else  // the node was destroyed by our request.
		{
			carried_resource += added_resource;
			type_count[type] += added_resource;
			//GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Carrying %d of %d"), carried_resource, max_resource));
		}
	}

}

void ARTSBUILDER::Check_Node_Status()
{
	if (IsValid(target_node))
	{

	}
	else if(carried_resource > 0)
	{
		ARTSStructure * droppoint = Get_Nearest_Dropoint();
		if (droppoint != NULL)
		{
			is_state_machine_active = true;
			target_struct = droppoint;
			state = DELIVERY_ON_ROUTE;
			RtsMoveToActor(target_struct);
		}
		else
		{
			state = IDLE;
		}
	}
	else
	{
		state = IDLE;
	}
}

// TODO:: IMPLEMENT ME
bool ARTSBUILDER::Node_Nearby(FVector check_local)  
{
	return false;
}



