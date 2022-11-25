// Fill out your copyright notice in the Description page of Project Settings.

#include "RTSBUILDER.h"
#include "RTS_Project/RTSFPS/GameSystems/ResourceSystem/Resource.h"
#include "RTS_Project/RTSFPS/RTS/Structures/RTSStructure.h"
#include "RTS_Project/RTSFPS/GameArchitecture/RTFPSMode.h"
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
	for (TPair<TSubclassOf<AResource>, uint32>& Elem : CarriedResources)
	{
		if (Elem.Value > 0U)
		{
			if (Structure->ScoreResource(Elem.Key, Elem.Value, this))
			{
				CarriedResources[Elem.Key] = 0;
				retval = true;
			}
		}
	}

	return(retval);
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

void ARTSBUILDER::StartMining(AResource * Node)
{
	/*Start the cooldown based off of current cooldown rate*/
	target_node = Node;
	GetWorldTimerManager().SetTimer(MineHandler, this, &ARTSBUILDER::MineResource, 1.0, false, MineInterval);
	bIsMining = true;
}

uint32 ARTSBUILDER::GetCurrentWeight() const
{
	return CurrentWeight;
}

uint32 ARTSBUILDER::GetMaxWeight() const
{
	return MaxCarryWeight;
}

void ARTSBUILDER::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ARTSBUILDER, bIsMining);
}

bool ARTSBUILDER::IsMining() const
{
	return(bIsMining && IsAlive());
}

void ARTSBUILDER::MineResource()
{
	/*
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
	*/
}

void ARTSBUILDER::AddResource(TSubclassOf<AResource> InResourceType, int InAmount)
{
	uint32* currentcount = CarriedResources.Find(InResourceType);
	if (currentcount != nullptr)
	{
		*currentcount += InAmount;
	}
	else
	{
		CarriedResources.Emplace(InResourceType, InAmount);
	}

	const AResource* resourcecdo = InResourceType.GetDefaultObject();
	const int resourceweight = resourcecdo->GetResourceWeight();
	CurrentWeight += (resourceweight * InAmount);
}

bool ARTSBUILDER::RemoveResource(const TSubclassOf<AResource> InResourceClass, int InAmount)
{
	bool retval = false;
	uint32* currentcount = CarriedResources.Find(InResourceClass);
	if (currentcount != nullptr)
	{
		*currentcount -= InAmount;
		retval = true;
	}
	if (retval == true)
	{
		const AResource* resourcecdo = InResourceClass.GetDefaultObject();
		const int resourceweight = resourcecdo->GetResourceWeight();
		CurrentWeight -= (resourceweight * InAmount);
	}

	return retval;
}

uint32 ARTSBUILDER::GetHeldResource(TSubclassOf<AResource> InResourceType) const
{
	uint32 retval = 0U;
	const uint32* currentcount = CarriedResources.Find(InResourceType);
	if (currentcount != nullptr)
	{
		retval = *currentcount;
	}
	return retval;
}

int ARTSBUILDER::CalculateGatherAmount(TSubclassOf<AResource> InResourceType) const
{
	int gatheramount = 0;
	const AResource* resourcecdo = InResourceType.GetDefaultObject();
	const int resourceweight = resourcecdo->GetResourceWeight();

	for (int i = 1; i < (MineAmount + 1); i++)
	{
		const int nextwieght = CurrentWeight + (resourceweight * i);
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
