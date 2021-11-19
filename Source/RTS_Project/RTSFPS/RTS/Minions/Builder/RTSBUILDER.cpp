// Fill out your copyright notice in the Description page of Project Settings.

#include "RTSBUILDER.h"
#include "RTS_Project/RTSFPS/GameObjects/Resource.h"
#include "RTS_Project/RTSFPS/RTS/Structures/RTSStructure.h"
#include "RTS_Project/GameArchitecture/Game/RTFPSMode.h"
#include "RTS_Project/RTSFPS/RTS/Minions/Builder/AI/BuilderAIController.h"

#include "Runtime/Engine/Public/TimerManager.h "
#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"


ARTSBUILDER::ARTSBUILDER()
{

}

bool ARTSBUILDER::DeliverResources(ARTSStructure* Structure)
{
	if (Structure == nullptr) return false;

	bool retval = false;
	for (TPair<TSubclassOf<AResource>, int>& Elem : CarriedResources)
	{
		if (Elem.Value > 0)
		{
			if (Structure->ScoreResource(Elem.Key, Elem.Value, this))
			{
				CarriedResources[Elem.Key] = 0;
				retval = true;
			}
		}
	}
	CalculateCurrentWeight();

	return(retval);
}

bool ARTSBUILDER::HasAssets()
{
	return(Super::HasAssets());
}

void ARTSBUILDER::ReleaseAssets()
{
	Super::ReleaseAssets();

	if (GetWorldTimerManager().IsTimerActive(MineHandler))
	{
		GetWorldTimerManager().ClearTimer(MineHandler);
		ABuilderAIController* AIC = Cast<ABuilderAIController>(GetController());
		if (AIC)
		{
			AIC->SendMineUpdateMessage();
		}
	}

	bIsMining = false;
}

bool ARTSBUILDER::CanInteract(AActor * Interactable)
{
	if(Cast<AResource>(Interactable) || Cast<ARTSStructure>(Interactable))
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
	return(MaxCarryWeight > CurrentWeight);
}

void ARTSBUILDER::StartMining(AResource * Node)
{
	/*Start the cooldown based off of current cooldown rate*/
	target_node = Node;
	GetWorldTimerManager().SetTimer(MineHandler, this, &ARTSBUILDER::MineResource, 1.0, false, MineInterval);
	bIsMining = true;
}

int ARTSBUILDER::GetCurrentWeight() const
{
	return CurrentWeight;
}

int ARTSBUILDER::GetMaxWeight() const
{
	return MaxCarryWeight;
}

int ARTSBUILDER::GetWeightof(TSubclassOf<AResource> ResourceType) const
{
	return 1;
}

void ARTSBUILDER::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ARTSBUILDER, bIsMining);
}

bool ARTSBUILDER::IsMining()
{
	return(bIsMining && IsAlive());
}

void ARTSBUILDER::MineResource()
{
	AResource * Node = Cast<AResource>(GetTarget());
	if(IsValid(Node) && Node == target_node) //verify that the current target and the target specified at start of mine operation are the same
	{
		TSubclassOf<AResource> ResourceType = Node->GetClass();
		int gatherAmount = CalculateGatherAmount(ResourceType);  // determine how much room we have to add.
		int addedResource = 0;
		addedResource = Node->Mine(gatherAmount);
		AddResource(ResourceType,addedResource);
	}

	ABuilderAIController * AIC = Cast<ABuilderAIController>(GetController());
	if (AIC)
	{
		AIC->SendMineUpdateMessage();
	}

	bIsMining = false;
	target_node =nullptr;
}

void ARTSBUILDER::AddResource(TSubclassOf<AResource> type, int amount)
{
	int* currentcount = CarriedResources.Find(type);
	if (currentcount != nullptr)
	{
		*currentcount += amount;
	}
	else
	{
		CarriedResources.Emplace(type, amount);
	}
	CalculateCurrentWeight();
}

void ARTSBUILDER::CalculateCurrentWeight()
{
	int weight = 0;
	for (TPair<TSubclassOf<AResource>, int>& Elem : CarriedResources)
	{
		weight += Elem.Value * GetWeightof(Elem.Key);
	}
	CurrentWeight = weight;
}

int ARTSBUILDER::CalculateGatherAmount(TSubclassOf<AResource> type) const
{
	int gatheramount = 0;
	int typeweight = GetWeightof(type);

	for (int i = 1; i < (MineAmount + 1); i++)
	{
		int nextwieght = CurrentWeight + (typeweight * i);
		if (nextwieght > MaxCarryWeight)
		{
			break;
		}
		else
		{
			gatheramount = i;
		}
	}
	return(gatheramount);
}



