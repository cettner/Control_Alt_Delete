// Fill out your copyright notice in the Description page of Project Settings.

#include "RTSBUILDER.h"
#include "RTS_Project/RTSFPS/GameSystems/ResourceSystem/Resource.h"
#include "RTS_Project/RTSFPS/RTS/Structures/RTSStructure.h"
#include "RTS_Project/RTSFPS/GameArchitecture/RTFPSMode.h"
#include "RTS_Project/RTSFPS/RTS/Minions/Builder/AI/BuilderAIController.h"

#include "Runtime/Engine/Public/TimerManager.h "
#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"

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

void ARTSBUILDER::StartMining(AResource * Node)
{
	/*Start the cooldown based off of current cooldown rate*/
	GetWorldTimerManager().SetTimer(MineHandler, this, &ARTSBUILDER::MineResource, 1.0, true, MineInterval);
	bIsMining = true;
}

bool ARTSBUILDER::StopMining()
{
	GetWorldTimerManager().ClearTimer(MineHandler);
	SetIsMining(false);
	return true;
}

uint32 ARTSBUILDER::GetCurrentWeight() const
{
	return CurrentWeight;
}

uint32 ARTSBUILDER::GetMaxWeight() const
{
	return MaxCarryWeight;
}

void ARTSBUILDER::SetIsMining(const bool InMiningState)
{
	bIsMining = InMiningState;
}

AResource* ARTSBUILDER::GetTargetResource() const
{
	AResource* retval = nullptr;
	const ARTSAIController* controller = GetController<ARTSAIController>();
	URTSMineResourceOrder * mineorder = controller->GetCurrentOrder<URTSMineResourceOrder>();

	if (IsValid(mineorder))
	{
		const FName& resourcenodekey = mineorder->GetResourceNodeKey();
		retval = Cast<AResource>(controller->GetBlackBoardKeyAsObject(resourcenodekey));
	}

	return retval;
}

const TSubclassOf<URTSTargetedOrder> ARTSBUILDER::GetDefaultOrderClass(const FHitResult& InHitContext) const
{
	TSubclassOf<URTSTargetedOrder> retval = nullptr;
	const AResource* resource = Cast<AResource>(InHitContext.GetActor());
	if (IsValid(resource))
	{
		retval = MineOrderClass;
	}
	else
	{
		retval = Super::GetDefaultOrderClass(InHitContext);
	}

	return retval;
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
	ARTSAIController* controller = GetController<ARTSAIController>();
	AResource* targetnode = GetTargetResource();
	if (IsValid(targetnode))
	{
		/*Copy class before as resrouce might not be valid after mining*/
		const TSubclassOf<AResource> resourceclass = targetnode->GetClass();
		ExtractResource(targetnode);
		if (!CanCarryMore(resourceclass))
		{
			StopMining();
			controller->SendAIMessage(ARTSAIController::AIMessageOrderRequest, FAIMessage::EStatus::Success, EAIMessageType::Task);
		}
	}
	else
	{
		controller->SendAIMessage(ARTSAIController::AIMessageOrderRequest, FAIMessage::EStatus::Failure, EAIMessageType::Task);
	}

}

bool ARTSBUILDER::ExtractResource(AResource* Node)
{
	bool retval = false;
	const TSubclassOf<AResource> resourceclass = Node->GetClass();
	const uint32 maxpull = GetResourceTillFull(resourceclass);

	if (maxpull > 0U)
	{
		uint32 actualpull = maxpull;
		if (maxpull > MineAmount)
		{
			actualpull = MineAmount;
		}
		const uint32 minedresource = Node->Mine(actualpull);
		AddResource(resourceclass, actualpull);
		retval = (actualpull > 0U);
	}

	return retval;
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
