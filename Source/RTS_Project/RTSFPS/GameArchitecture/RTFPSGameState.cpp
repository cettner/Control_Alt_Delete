// Fill out your copyright notice in the Description page of Project Settings.

#include "RTFPSGameState.h"
#include "RTS_Project/RTSFPS/FPS/Death/RespawnSelectionPawn.h"
#include "RTS_Project/RTSFPS/RTS/Structures/RTSStructure.h"
#include "RTS_Project/RTSFPS/Shared/Upgrades/RTSUpgrade.h"

#include "UObject/CoreNetTypes.h"
#include "Engine/ActorChannel.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

ARTFPSGameState::ARTFPSGameState(const FObjectInitializer &FOI) : Super(FOI)
{
	RTSObjects = TArray<IRTSObjectInterface*>();
	/*Required for Replication of PurchaseOrders*/
	bReplicateUsingRegisteredSubObjectList = true;
}

void ARTFPSGameState::RefreshAllUnits()
{

	RTSObjects.Reset();

	const UWorld * world = GetWorld();
	TArray<AActor*> rtsactors = TArray<AActor*>();
	UGameplayStatics::GetAllActorsWithInterface(world, URTSObjectInterface::StaticClass(), rtsactors);

	if (HasAuthority())
	{
		for (int i = 0; i < rtsactors.Num(); i++)
		{
			AddRTSObjectToTeam(Cast<IRTSObjectInterface>(rtsactors[i]));
			RegisterRTSObject(Cast<IRTSObjectInterface>(rtsactors[i]));
		}
	}
	else
	{
		for (int i = 0; i < rtsactors.Num(); i++)
		{
			RegisterRTSObject(Cast<IRTSObjectInterface>(rtsactors[i]));
		}
	}
}

void ARTFPSGameState::OnUnitDeath(IRTSObjectInterface * Unit)
{
	if (Unit && IsTeamValid(Unit->GetTeam()))
	{
		ARTSMinion * minion = Cast<ARTSMinion>(Unit);

		if (minion)
		{
			/*Remove the Object From All PLayers who share its Team*/
			if (!RemoveRTSObjectFromTeam(Unit))
			{
				UE_LOG(LogTemp, Warning, TEXT("[ARTFPSGameState::OnUnitDeath] Minion Removal Unsuccessful!"));
			}

			/*If A Player Died, Assign a New Pawn to await and choose Respawn*/
			AFPSServerController* PC = minion->GetController<AFPSServerController>();
			if (PC)
			{
				HandlePlayerDeath(PC);
			}
		}
	}
}

void ARTFPSGameState::HandlePlayerDeath(AFPSServerController* Controller)
{
	/*todo, respawnstate should be placed in the playerstate, not the controller*/
	Controller->OnPawnDeath();
	Controller->UnPossess();
	UWorld* World = GetWorld();

	if (World == nullptr) return;

	FActorSpawnParameters RespawnActorParams;
	RespawnActorParams.bNoFail = true;
	RespawnActorParams.Owner = Controller;

	ADefaultPlayerState* PS = Controller->GetPlayerState<ADefaultPlayerState>();

	ARespawnSelectionPawn * Respawnactor =  World->SpawnActor<ARespawnSelectionPawn>(ARespawnSelectionPawn::StaticClass(), RespawnActorParams);
	if (Respawnactor)
	{
		Controller->Possess(Respawnactor);
	}

}

const TMap<TSubclassOf<UObject>, FReplicationResourceMap> ARTFPSGameState::GetAllDefaultUnitPrices() const
{
	TMap<TSubclassOf<UObject>, FReplicationResourceMap> retval = TMap<TSubclassOf<UObject>, FReplicationResourceMap>();
	for (int i = 0; i < PurchaseOrdersRep.Num(); i++)
	{
		if (IsValid(PurchaseOrdersRep[i]))
		{
			retval.Emplace(PurchaseOrdersRep[i]->GetPurchaseClass(), PurchaseOrdersRep[i]->GetPurchasePrice());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ARTFPSGameState::GetAllDefaultUnitPrices PurchaseOrder from replication was null at time of request"));
		}
	}


	return retval;
}

void ARTFPSGameState::SetUnitPriceMap(const TMap<TSubclassOf<UObject>, FReplicationResourceMap> InMap)
{
	if (HasAuthority())
	{
		for (TPair<TSubclassOf<UObject>, FReplicationResourceMap> elem : InMap)
		{
			URTSResourcePurchaseOrder* purchaseorder = NewObject<URTSResourcePurchaseOrder>(this, URTSResourcePurchaseOrder::StaticClass());
			purchaseorder->SetPurchaseClass(elem.Key);
			purchaseorder->SetPrice(elem.Value);
			AddReplicatedSubObject(purchaseorder, ELifetimeCondition::COND_InitialOnly);
			PurchaseOrdersRep.Emplace(purchaseorder);
		}
		OnRep_PurchaseOrdersRep();
	}
}

void ARTFPSGameState::AddRTSObjectToTeam(IRTSObjectInterface * const InObject)
{
	const int teamid = InObject->GetTeam();
	if(IsTeamValid(teamid))
	{
		ATeamResourceState *ts = GetTeamState<ATeamResourceState>(teamid);
		ts->AddRTSUnit(InObject);
	}
}

bool ARTFPSGameState::RemoveRTSObjectFromTeam(IRTSObjectInterface * InObject)
{
	bool retval = false;
	const int teamid = InObject->GetTeam();

	if(IsTeamValid(teamid))
	{
		ATeamResourceState* ts = GetTeamState<ATeamResourceState>(teamid);
		retval = ts->RemoveRTSUnit(InObject);
	}
	return retval;
}

void ARTFPSGameState::RegisterRTSObject(IRTSObjectInterface* InObject)
{
	RTSObjects.AddUnique(InObject);
}

void ARTFPSGameState::UnRegisterRTSObject(IRTSObjectInterface* InObject)
{
	RTSObjects.Remove(InObject);
}

const TArray<IRTSObjectInterface*>& ARTFPSGameState::GetRegisteredRTSObjects() const
{
	return RTSObjects;
}

TArray<URTSResourcePurchaseOrder*> ARTFPSGameState::GetPurchaseOrders(const TArray<TSubclassOf<UObject>> InPurchaseObjects) const
{
	TArray<URTSResourcePurchaseOrder*> retval = TArray<URTSResourcePurchaseOrder*>();
	
	for (int i = 0; i < InPurchaseObjects.Num(); i++)
	{
		checkf(InPurchaseObjects[i], TEXT("ARTFPSGameState::GetPurchaseOrders : ARequestedPurchaseClass was Null!"))
		const TSubclassOf<UObject> requestedobject = InPurchaseObjects[i];

		URTSResourcePurchaseOrder*const * purchaseorderptr = PurchaseOrders.Find(requestedobject);
		if (purchaseorderptr != nullptr)
		{
			retval.Emplace(*purchaseorderptr);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ARTFPSGameState::GetPurchaseOrders %s Not Found In Available PurchaseList"), *InPurchaseObjects[i].Get()->GetName());
		}
	}

	return retval;
}

bool ARTFPSGameState::PurchaseExpUpgrade(const TSubclassOf<UUpgrade> PurchaseClass, IExpAccumulatorInterface * Purchaser, IUpgradableInterface * ToApply) const
{
	bool retval = false;

	if (Purchaser->SpendUpgradePoints(1U))
	{
		retval = ToApply->InstallUpgrade(PurchaseClass);
	}

	return retval;
}

bool ARTFPSGameState::RefundExpUpgrade(const TSubclassOf<UUpgrade> InRefundClass, IExpAccumulatorInterface* Purchaser, IUpgradableInterface* ToApply) const
{
	bool retval = false;

	if (ToApply->UninstallUpgrade(InRefundClass))
	{
		Purchaser->RefundUpgradePoints(1U);
		retval = true;
	}

	return retval;
}

bool ARTFPSGameState::TeamInitialize(ADefaultMode* GameMode)
{
	bool result = Super::TeamInitialize(GameMode);
	
	UWorld* world = GetWorld();
	const ARTFPSMode * gm = Cast<ARTFPSMode>(GameMode);

	for (int32 i = 0; i < TeamStates.Num(); i++)
	{
		const ATeamState* ts = TeamStates[i];
		const int32 teamid = ts->GetTeam();
		
		ARTSUpgradeManager * upgrademanager = world->SpawnActor<ARTSUpgradeManager>(ARTSUpgradeManager::StaticClass());
		upgrademanager->SetOwner(this);
		upgrademanager->SetTeamID(teamid);
		UpgradeManagers.Emplace(upgrademanager);
	}

	/*Give Each Player their Starting Resources and Unit Pricing*/
	/*Set for Server, Clients load from the GameMode CDO on GM class replication*/
	const TMap<TSubclassOf<UObject>, FReplicationResourceMap> costs = gm->GetDefaultUnitCosts();
	SetUnitPriceMap(costs);

	/*Find the Units in the gameWorld and add them to thier respective teams*/
	RefreshAllUnits();

	return result;
}

void ARTFPSGameState::PlayerGameDataInit(ADefaultPlayerState * Player)
{
	
}

void ARTFPSGameState::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (!HasAuthority())
	{
		RefreshAllUnits();
	}
}

void ARTFPSGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ARTFPSGameState, PurchaseOrdersRep);
}

void ARTFPSGameState::OnRep_PurchaseOrdersRep()
{
	PurchaseOrders.Reset();
	for (int i = 0; i < PurchaseOrdersRep.Num(); i++)
	{
		if (IsValid(PurchaseOrdersRep[i]))
		{
			PurchaseOrders.Emplace(PurchaseOrdersRep[i]->GetPurchaseClass(), PurchaseOrdersRep[i]);
		}
	}
}






