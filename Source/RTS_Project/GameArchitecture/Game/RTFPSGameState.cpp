// Fill out your copyright notice in the Description page of Project Settings.

#include "RTFPSGameState.h"
#include "RTFPSPlayerState.h"
#include "RTS_Project/RTSFPS/FPS/Death/RespawnSelectionPawn.h"
#include "RTS_Project/RTSFPS/RTS/Structures/RTSStructure.h"

#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

ARTFPSGameState::ARTFPSGameState(const FObjectInitializer &FOI) : Super(FOI)
{
	AllUnits = TArray<RTSTeamUnits>();
}

int ARTFPSGameState::NumRTSPlayers(int Team_Index)
{
	int retval = 0;
	if(IsTeamValid(Team_Index))
	{
		for (int i = 0; i < Teams[Team_Index].Num(); i++)
		{
			ARTFPSPlayerState * PS = Cast<ARTFPSPlayerState>(Teams[Team_Index][i]);
			if (PS && PS->IsRTSPlayer())
			{
				retval++;
			}
		}
	}
	return(retval);
}

void ARTFPSGameState::RefreshAllUnits()
{
	UWorld * world = GetWorld();
	for (TActorIterator<ARTSMinion> Itr(world); Itr; ++Itr)
	{
		IRTSObjectInterface * rtsobj = *Itr;
		AddRTSObjectToTeam(rtsobj);
	}
	for (TActorIterator<ARTSStructure> Itr(world); Itr; ++Itr)
	{
		IRTSObjectInterface * rtsobj = *Itr;
		AddRTSObjectToTeam(rtsobj);
	}
}

void ARTFPSGameState::OnUnitDeath(IRTSObjectInterface * Unit)
{
	if (Unit && IsTeamValid(Unit->GetTeam()))
	{
		ARTSMinion * minion = Cast<ARTSMinion>(Unit);
		ARTSStructure * structure = Cast<ARTSStructure>(Unit);

		if (minion)
		{
			if (AllUnits[Unit->GetTeam()].Minions.Remove(minion) != (int32)1)
			{
				UE_LOG(LogTemp, Warning, TEXT("[ARTFPSGameState::OnUnitDeath] Minion Removal Unsuccessful!"));
			}

			AFPSServerController* PC = minion->GetController<AFPSServerController>();
			if (PC)
			{
				HandlePlayerDeath(PC);
			}
		}

		else if(structure)
		{
			if(AllUnits[Unit->GetTeam()].Structures.Remove(structure) != (int32)1)
			{
				UE_LOG(LogTemp, Warning, TEXT("[ARTFPSGameState::OnUnitDeath] Structure Removal was Unsuccessful!"));
			}
		}
	}
}

void ARTFPSGameState::HandlePlayerDeath(AFPSServerController* Controller)
{
	Controller->OnPawnDeath();
	Controller->UnPossess();
	UWorld* World = GetWorld();

	if (World == nullptr) return;

	FActorSpawnParameters RespawnActorParams;
	RespawnActorParams.bNoFail = true;
	RespawnActorParams.Owner = Controller;

	ADefaultPlayerState* PS = Controller->GetPlayerState<ADefaultPlayerState>();
	TArray<ARTSStructure*> structures = GetAllStructuresOfTeam(PS->TeamID);

	ARespawnSelectionPawn * Respawnactor =  World->SpawnActor<ARespawnSelectionPawn>(ARespawnSelectionPawn::StaticClass(), RespawnActorParams);
	if (Respawnactor)
	{
		Controller->Possess(Respawnactor);
	}

}

void ARTFPSGameState::HandleStructureMinionSpawn(ARTSStructure* SpawningStructure, FStructureQueueData SpawnData)
{
	UWorld* World = GetWorld();

	ARTSMinion* Minion = World->SpawnActorDeferred<ARTSMinion>(SpawnData.SpawnClass,FTransform(),nullptr,nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);

	AController* PC = SpawnData.RecieveingController;

	if (PC && Minion)
	{
		Minion->SetTeam(PC->GetPlayerState<ADefaultPlayerState>()->TeamID);
		APawn* respawnpawn = PC->GetPawn();
		PC->UnPossess();
		respawnpawn->Destroy();
		PC->Possess(Minion);
	}
	else
	{
		Minion->SetTeam(SpawningStructure->GetTeam());
		/*Set Team Colors on listen Server*/
		Minion->OnRep_TeamID();
	}

	UGameplayStatics::FinishSpawningActor(Minion, FTransform());
	AddRTSObjectToTeam(Minion);
}

void ARTFPSGameState::HandleStructureSpawn(TSubclassOf<AActor> StructureClass, FTransform SpawnTransform, ADefaultPlayerController* InvokedController)
{
	UWorld* world = GetWorld();
	check(world);

	ARTSStructure* structure = world->SpawnActorDeferred<ARTSStructure>(StructureClass, SpawnTransform);

	check(structure);

	const int teamid = InvokedController->GetTeamID();
	structure->SetTeam(teamid);

	
	UGameplayStatics::FinishSpawningActor(structure,SpawnTransform);
	AddRTSObjectToTeam(structure);
}

TArray<ARTSMinion*> ARTFPSGameState::GetAllMinionsOfTeam(int teamindex) const
{
    TArray<ARTSMinion*> minions = TArray<ARTSMinion*>();
	if (IsTeamValid(teamindex))
	{
		minions = AllUnits[teamindex].Minions;
	}

	return minions;
}

TArray<ARTSStructure *> ARTFPSGameState::GetAllStructuresOfTeam(int teamindex) const
{
	TArray<ARTSStructure*> structures = TArray<ARTSStructure*>();
	if (IsTeamValid(teamindex))
	{
		structures = AllUnits[teamindex].Structures;
	}

	return structures;
}

TArray<TSubclassOf<AResource>> ARTFPSGameState::GetResourceTypes() const
{
	TArray<TSubclassOf<AResource>> ResourceTypes = TArray<TSubclassOf<AResource>>();

	for (TObjectIterator<AResource> Itr; Itr; ++Itr)
	{
		AResource* Resource = *Itr;
		ResourceTypes.AddUnique(Resource->GetClass());
	}

	return ResourceTypes;
}

TArray<FResourceUIData> ARTFPSGameState::GetMapResourceInfo() const
{
	return MapResourceInfo;
}

void ARTFPSGameState::InitializeResources(ARTFPSMode * GameMode)
{
	check(GameMode);

	/*Scan the Map and Add one of each type of Resource to the list of available resources*/
	TArray<TSubclassOf<AResource>> MapResources = GetResourceTypes();
	if (!InitializeMapResourceInfo(MapResources))
	{
		UE_LOG(LogTemp, Warning, TEXT("ARTFPSGameState::InitializeResources : Failed to Obtain Map Resources"));
	}

	for (int i = 0; i < GameMode->GetNumTeams(); i++)
	{
		FReplicationResourceMap Resources;

		for (int k = 0; k < MapResources.Num(); k++)
		{
			int startingval = GameMode->GetStartingResources(MapResources[k]);
			Resources.Emplace(MapResources[k], startingval);
			GameMode->GetStartingResources(MapResources[k]);
		}
		TeamResources.Emplace(Resources);
	}

	UnpackUnitPriceMap(GameMode->GetDefaultUnitCosts());
}

bool ARTFPSGameState::AddTeamResource(int TeamID, TSubclassOf<AResource> ResourceClass, int amount)
{
	bool retval = false;
	if (IsTeamValid(TeamID))
	{
		const int* currentval = TeamResources[TeamID].Find(ResourceClass);
		if (currentval != nullptr)
		{
			int newval = *currentval + amount;
			TeamResources[TeamID].Emplace(ResourceClass, newval);

			retval = true;
		}
	}
	return retval;
}

bool ARTFPSGameState::IsTeamResourceAvailable(int TeamID, TSubclassOf<AResource> ResourceClass, int requestedamount) const
{
	bool retval = false;
	if (IsTeamValid(TeamID))
	{
		auto currentval = TeamResources[TeamID].Find(ResourceClass);
		if (currentval != nullptr && *currentval >= requestedamount)
		{
			retval = true;
		}
	}
	return retval;
}

bool ARTFPSGameState::IsTeamResourceAvailable(int TeamID, FReplicationResourceMap RequestedAmount) const
{
	bool retval = true;
	for (TPair<TSubclassOf<AResource>, int> Elem : RequestedAmount.GetMap())
	{
		retval &= IsTeamResourceAvailable(TeamID, Elem.Key, Elem.Value);
	}

	return retval;
}

bool ARTFPSGameState::RemoveTeamResource(int TeamID, TSubclassOf<AResource> ResourceClass, int amount)
{
	bool retval = false;
	if (IsTeamValid(TeamID))
	{
		const int* currentval = TeamResources[TeamID].Find(ResourceClass);
		if (currentval != nullptr && *currentval >= amount)
		{
			int newval = *currentval - amount;
			TeamResources[TeamID].Emplace(ResourceClass, newval);
			retval = true;
		}
	}
	return retval;
}

bool ARTFPSGameState::RemoveTeamResource(int TeamID, TMap<TSubclassOf<AResource>, int> ResourceCosts)
{
	if (!IsTeamValid(TeamID)) return(false);

	/*We can Only remove if we can afford all costs*/
	bool CanAfford = true;
	for (TPair<TSubclassOf<AResource>, int>  Elem : ResourceCosts)
	{
		CanAfford &= IsTeamResourceAvailable(TeamID,Elem.Key,Elem.Value);
	}

	if (!CanAfford) return(false);


	bool ResourcesRemoved = true;
	for (TPair<TSubclassOf<AResource>, int> Elem : ResourceCosts)
	{
		ResourcesRemoved &= RemoveTeamResource(TeamID, Elem.Key, Elem.Value);
	}

	if (!ResourcesRemoved)
	{
		UE_LOG(LogTemp, Warning, TEXT("[ARTFPSGameState::RemoveTeamResource] Failed to Remove all Team Resources!"));
	}

	return ResourcesRemoved;
}

void ARTFPSGameState::UnpackUnitPriceMap(TMap<TSubclassOf<AActor>, FReplicationResourceMap> GameModePrices)
{
	for (const TPair<TSubclassOf<AActor>, FReplicationResourceMap>& pair : GameModePrices)
	{
		PurchasableUnits.Emplace(pair.Key);
		UnitCosts.Emplace(pair.Value);
	}
}

void ARTFPSGameState::AddRTSObjectToTeam(IRTSObjectInterface * InObject)
{
	const int teamid = InObject->GetTeam();
	if(IsTeamValid(teamid))
	{
		ARTSMinion * isminion = Cast<ARTSMinion>(InObject); 
		ARTSStructure * isstructure = Cast<ARTSStructure>(InObject);
		/**/
		if(isminion != nullptr)
		{ 
			AllUnits[teamid].Minions.AddUnique(isminion);
			for(int i = 0; i < Teams[teamid].Num(); i++)
			{
				
			}
		}
		else if(isstructure != nullptr)
		{
			/*Add in the units on the server*/
			AllUnits[teamid].Structures.AddUnique(isstructure);
			for(int i = 0; i < Teams[teamid].Num(); i++)
			{
				/*For each player, update the list of Structures unique to thier team*/
				ARTFPSPlayerState * ps = Cast<ARTFPSPlayerState>(Teams[teamid][i]);
				check(ps);
				ps->SetTeamStructures(AllUnits[teamid].Structures);
			}
		}
	}

}

bool ARTFPSGameState::RemoveRTSObjectFromTeam(IRTSObjectInterface * InObject)
{
	return false;
}

int ARTFPSGameState::GetTeamResourceValue(int TeamID, TSubclassOf<AResource> ResourceClass) const
{
	int retval = -1;
	if (IsTeamValid(TeamID) && TeamResources[TeamID].Find(ResourceClass))
	{
		retval = *TeamResources[TeamID].Find(ResourceClass);
	}

	return(retval);
}

bool ARTFPSGameState::PurchaseUnit(TSubclassOf<AActor> PurchaseClass, ARTSPlayerController * Purchaser)
{
	bool retval = false;
	if (IsUnitPurchaseable(PurchaseClass, Purchaser))
	{
		FReplicationResourceMap costs = GetUnitPrice(PurchaseClass);
		retval = RemoveTeamResource(Purchaser->GetTeamID(), costs.GetMap());
	}

	return(retval);

}

FReplicationResourceMap ARTFPSGameState::RefundUnit(TSubclassOf<AActor> RefundClass, ARTSPlayerController * Purchaser)
{
	return FReplicationResourceMap();
}

bool ARTFPSGameState::IsUnitPurchaseable(TSubclassOf<AActor> PurchaseClass, AController* Purchaser) const
{
	bool ispurchasable = PurchasableUnits.IndexOfByKey(PurchaseClass) != INDEX_NONE;
	ispurchasable &= (PurchasableUnits.Num() == UnitCosts.Num());

	return (ispurchasable);
}

FReplicationResourceMap ARTFPSGameState::GetUnitPrice(TSubclassOf<AActor> PurchaseClass) const
{
	int purchaseindex = PurchasableUnits.IndexOfByKey(PurchaseClass);
	FReplicationResourceMap retval = FReplicationResourceMap();
	bool replicationvalid = (PurchasableUnits.Num() == UnitCosts.Num());

	if ((purchaseindex != INDEX_NONE) && replicationvalid)
	{
		retval = UnitCosts[purchaseindex];
	}

	return retval;
}

bool ARTFPSGameState::ScoreResource(TSubclassOf<AResource> ResourceType, int Amount, IRTSObjectInterface* Donar)
{
	return AddTeamResource(Donar->GetTeam(),ResourceType,Amount);
}

bool ARTFPSGameState::TeamInitialize(ADefaultMode* GameMode)
{
	bool result = Super::TeamInitialize(GameMode);

	ARTFPSMode * Game = Cast<ARTFPSMode>(GameMode);
	check(Game);
	
	/*Give Each Player their Starting Resources and Unit Pricing*/
	InitializeResources(Game);
	
	/*For Each Team, Create a Set Of Units*/
	for (int i = 0; i < GameMode->GetNumTeams(); i++)
	{
		RTSTeamUnits newunitteam;
		AllUnits.Emplace(newunitteam);
	}

	/*Find the Units in the gameWorld and add them to thier respective teams*/
	RefreshAllUnits();

	return result;
}

void ARTFPSGameState::PlayerGameDataInit(APlayerState * Player)
{
	ARTFPSPlayerState * ps = Cast<ARTFPSPlayerState>(Player);
	const int teamid = ps->TeamID;

	TArray<ARTSStructure *> initstructures = GetAllStructuresOfTeam(teamid);
	ps->SetTeamStructures(initstructures);
	
}

bool ARTFPSGameState::InitializeMapResourceInfo(TArray<TSubclassOf<AResource>> ResourceClasses)
{
	bool retval = true;
	for (int i = 0; i < ResourceClasses.Num(); i++)
	{
		TSubclassOf<AResource> SearchClass = ResourceClasses[i];

		for (TActorIterator<AResource> It(GetWorld(), SearchClass); It; ++It)
		{
			AResource* ResourceType = *It;
			MapResourceInfo.Add(ResourceType->GetUIData());
			break;
		}
	}

	return(retval);
}

void ARTFPSGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ARTFPSGameState, TeamResources);
	DOREPLIFETIME_CONDITION(ARTFPSGameState, MapResourceInfo, COND_InitialOnly);
	DOREPLIFETIME_CONDITION(ARTFPSGameState, UnitCosts, COND_InitialOnly);
	DOREPLIFETIME_CONDITION(ARTFPSGameState, PurchasableUnits, COND_InitialOnly);
}





