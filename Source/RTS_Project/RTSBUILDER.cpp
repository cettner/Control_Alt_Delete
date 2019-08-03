// Fill out your copyright notice in the Description page of Project Settings.

#include "RTSBUILDER.h"
#include "Resource.h"
#include "RTSStructure.h"
#include "GameFramework/PlayerController.h"
#include "RTSPlayerController.h"
#include "Public/BuilderAIController.h"
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
	carried_resource = 0;
}

bool ARTSBUILDER::Drop_Point_Available()
{
	return (true);
}

bool ARTSBUILDER::HasAssets()
{
	return(Super::HasAssets());
}


void ARTSBUILDER::ReleaseAssets()
{
	Super::ReleaseAssets();
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

bool ARTSBUILDER::CanCarryMore()
{
	return(max_resource > carried_resource);
}


void ARTSBUILDER::StartMining(AResource * Node)
{
	/*Start the cooldown based off of current cooldown rate*/	
	target_node = Node;
	GetWorldTimerManager().SetTimer(Mine_Handler, this, &ARTSBUILDER::Mine_Resource, 1.0, false, mine_interval);
	node_timer_set = true;
}

bool ARTSBUILDER::CanMine()
{
	return(!node_timer_set);
}

void ARTSBUILDER::Mine_Resource()
{
	AResource * Node = Cast<AResource>(GetTarget());
	if(IsValid(Node) && Node == target_node) //verify that the current target and the target specified at start of mine operation are the same
	{
		int gather_amount = (max_resource - carried_resource);  // determine how much room we have to add.
		int added_resource = 0;
		Resource_Types type = NULL_TYPE;

		if (gather_amount < mine_amount)  //ask the node for less if we can only fit that much
		{	
			added_resource = Node->Mine(gather_amount,type);
		}
		else    // we can ask for the full value
		{
			gather_amount = mine_amount; 
			added_resource = Node->Mine(gather_amount,type);
		}
	
		carried_resource += added_resource;
		type_count[type] += added_resource;

		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("Carrying %d of %d"), carried_resource, max_resource));
	}

	ABuilderAIController * AIC = Cast<ABuilderAIController>(GetController());
	if (AIC)
	{
		AIC->SendMineUpdateMessage();
	}

	node_timer_set = false;
	target_node =nullptr;
}




// TODO:: IMPLEMENT ME
bool ARTSBUILDER::Node_Nearby(FVector check_local)  
{
	return false;
}



