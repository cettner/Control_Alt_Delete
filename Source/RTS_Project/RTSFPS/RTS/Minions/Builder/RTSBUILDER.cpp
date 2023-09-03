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
	AIConfig.SightAffiliation.bDetectNeutrals = true;
	AIConfig.SightAffiliation.bDetectEnemies = false;
	AIConfig.SightAffiliation.bDetectFriendlies = false;

	AISenseClasses.Emplace(UAISense_Sight::StaticClass());

	ResourceComp = CreateDefaultSubobject<UResourceGathererComponent>(TEXT("ResourceComp"));
}

bool ARTSBUILDER::DeliverResources(ARTSStructure* Structure)
{
	if (Structure == nullptr) return false;

	bool retval = Structure->ScoreResource(this);

	return(retval);
}

void ARTSBUILDER::StartMining(AResource * Node)
{
	/*Start the cooldown based off of current cooldown rate*/
	GetWorldTimerManager().SetTimer(MineHandler, this, &ARTSBUILDER::MineResource, 1.0, true, MineInterval);
	NodeDestroyedHandle = Node->OnResourceDestroyedDelegate.AddUFunction(this, "OnResourceNodeDepleted"); 
	bIsMining = true;
}

bool ARTSBUILDER::StopMining()
{
	GetWorldTimerManager().ClearTimer(MineHandler);
	NodeDestroyedHandle.Reset();
	SetIsMining(false);
	return true;
}

uint32 ARTSBUILDER::GetCurrentWeight() const
{
	return ResourceComp->GetCurrentWeight();
}

uint32 ARTSBUILDER::GetMaxWeight() const
{
	return ResourceComp->GetMaxWeight();
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
		StopMining();
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

void ARTSBUILDER::OnResourceNodeDepleted()
{
	ARTSAIController* controller = GetController<ARTSAIController>();
	StopMining();
	controller->SendAIMessage(ARTSAIController::AIMessageOrderRequest, FAIMessage::EStatus::Failure, EAIMessageType::Task);
}

void ARTSBUILDER::AddResource(TSubclassOf<AResource> InResourceType, int InAmount)
{
	/*
	CarriedResources.Increment(InResourceType,InAmount);

	const AResource* resourcecdo = InResourceType.GetDefaultObject();
	const int resourceweight = resourcecdo->GetResourceWeight();
	CurrentWeight += (resourceweight * InAmount);
	*/
	ResourceComp->AddResource(InResourceType, InAmount);
}

bool ARTSBUILDER::RemoveResource(const TSubclassOf<AResource> InResourceType, int InAmount)
{
	/*
	bool retval = CarriedResources.Decrement(InResourceType, InAmount);
	const AResource* resourcecdo = InResourceType.GetDefaultObject();
	const int resourceweight = resourcecdo->GetResourceWeight();
	CurrentWeight -= (resourceweight * InAmount);
	*/

	return 	ResourceComp->RemoveResource(InResourceType, InAmount);
}

FReplicationResourceMap ARTSBUILDER::GetAllHeldResources() const
{
	return CarriedResources;
}
