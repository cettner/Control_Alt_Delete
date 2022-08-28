// Fill out your copyright notice in the Description page of Project Settings.

#include "RTFPSGameState.h"
#include "RTS_Project/RTSFPS/FPS/Death/RespawnSelectionPawn.h"
#include "RTS_Project/RTSFPS/RTS/Structures/RTSStructure.h"
#include "RTS_Project/RTSFPS/Shared/Upgrades/RTSUpgrade.h"

#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

ARTFPSGameState::ARTFPSGameState(const FObjectInitializer &FOI) : Super(FOI)
{
	AllUnits = TArray<RTSTeamUnits>();
	RTSObjects = TArray<IRTSObjectInterface*>();
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

	RTSObjects.Reset();

	const UWorld * world = GetWorld();
	TArray<AActor*> rtsactors = TArray<AActor*>();
	UGameplayStatics::GetAllActorsWithInterface(world, URTSObjectInterface::StaticClass(), rtsactors);

	if (HasAuthority())
	{
		/*TODO, Add way to add back in Upgrades if this is called during runtime*/
		for (int i = 0; i < AllUnits.Num(); i++)
		{
			AllUnits[i].Minions.Reset();
			AllUnits[i].Structures.Reset();
			AllUnits[i].Upgrades.Reset();
		}

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
	Controller->OnPawnDeath();
	Controller->UnPossess();
	UWorld* World = GetWorld();

	if (World == nullptr) return;

	FActorSpawnParameters RespawnActorParams;
	RespawnActorParams.bNoFail = true;
	RespawnActorParams.Owner = Controller;

	ADefaultPlayerState* PS = Controller->GetPlayerState<ADefaultPlayerState>();
	TArray<ARTSStructure*> structures = GetAllStructuresOfTeam(PS->GetTeamID());

	ARespawnSelectionPawn * Respawnactor =  World->SpawnActor<ARespawnSelectionPawn>(ARespawnSelectionPawn::StaticClass(), RespawnActorParams);
	if (Respawnactor)
	{
		Controller->Possess(Respawnactor);
	}

}

bool ARTFPSGameState::SpawnObjectFromStructure(ARTSStructure* SpawningStructure, const FStructureQueueData SpawnData)
{
	bool retval = false;
	UWorld* World = GetWorld();
	/*Check if the Object is a Minion, or an Upgrade*/
	const bool isminion = SpawnData.SpawnClass.Get()->IsChildOf(ARTSMinion::StaticClass());
	if (isminion == true)
	{
		FVector minionextent;
		FVector minionorigin;

		ARTSMinion * Minion = World->SpawnActorDeferred<ARTSMinion>(SpawnData.SpawnClass, SpawningStructure->GetTransform(), nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding);
		Minion->GetActorBounds(true, minionorigin, minionextent);
		const FTransform spawntransform = SpawningStructure->FindActorSpawnLocation(minionextent);

		AController* pc = SpawnData.RecieveingController;
		const AFPSPlayerState* ps;
		
		if (pc != nullptr)
		{
			ps = pc->GetPlayerState<AFPSPlayerState>();
		}
		else 
		{
			ps = nullptr;
		}



		/*If its a minion, Check whether the purchasing controller will be assuming control of the Pawn*/
		const bool isvalidforpossession = (Cast<ACommander>(Minion) != nullptr) && (ps != nullptr);
		if ((isvalidforpossession == true) && (ps->GetRespawnState() == EPlayerReswpawnState::AWAITINGRESPAWN))
		{
			Minion->SetTeam(pc->GetPlayerState<ADefaultPlayerState>()->GetTeamID());
			/*Apply Upgrades that are global to the team*/
			ApplyGlobalUpgrades(Minion);
			/*Apply Upgrades that are specific to the player*/
			ApplyPlayerUpgrades(Minion, pc->GetPlayerState<AFPSPlayerState>());
			
			/*Get And Destroy the Pawn the player was Using and give them the new one*/
			APawn* respawnpawn = pc->GetPawn();
			pc->UnPossess();
			respawnpawn->Destroy();
			pc->Possess(Minion);
		}
		else
		{
			Minion->SetTeam(SpawningStructure->GetTeam());
			/*Apply Upgrades that are global to the team*/
			ApplyGlobalUpgrades(Minion);
			/*Set Team Colors on listen Server*/
			if (GetNetMode() == NM_ListenServer)
			{
				Minion->OnRep_TeamID();
			}
		}

		UGameplayStatics::FinishSpawningActor(Minion, spawntransform);
		retval = IsValid(Minion);

		if (IsValid(Minion))
		{
			AddRTSObjectToTeam(Minion);
			retval = true;
		}
		else
		{

		}

	}
	else
	{
		/*Handle A new Upgrade Purchase*/
		const int teamid = SpawningStructure->GetTeam();
		URTSUpgrade * const defaultupgrade = Cast<URTSUpgrade>(SpawnData.SpawnClass.GetDefaultObject());
		const TSubclassOf<UUpgrade> upgradeclass = TSubclassOf<UUpgrade>(defaultupgrade->GetClass());
		const AController* PC = SpawnData.RecieveingController;
		TArray<AActor*> minionactors = TArray<AActor*>();

		if(defaultupgrade->IsGlobal() && defaultupgrade->IsPersistent())
		{
			/*Upgrade All Existing Minions On the Map*/
			for (int i = 0; i < AllUnits[teamid].Minions.Num(); i++)
			{
				if (defaultupgrade->CanUpgrade(AllUnits[teamid].Minions[i]))
				{
					minionactors.Emplace(AllUnits[teamid].Minions[i]);
				}
			}

			UpgradeManager->CheckAndDispatchUpgrade(upgradeclass, minionactors);
			AddRTSObjectToTeam(defaultupgrade);
		}
		else if(!defaultupgrade->IsGlobal() && defaultupgrade->IsPersistent())
		{
			ARTSMinion * playerpawn = PC->GetPawn<ARTSMinion>();
			if(IsValid(playerpawn) && defaultupgrade->CanUpgrade(playerpawn))
			{
				minionactors.Emplace(playerpawn);
			}
			UpgradeManager->CheckAndDispatchUpgrade(upgradeclass, minionactors);
			AFPSPlayerState * ps = PC->GetPlayerState<AFPSPlayerState>();
			ps->AddUpgrade(upgradeclass);
		}
		retval = true;
	}

	return(retval);
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

void ARTFPSGameState::ApplyGlobalUpgrades(ARTSMinion * Minion) const
{
	const int teamid = Minion->GetTeam();
	for (int i = 0; i < AllUnits[teamid].Upgrades.Num(); i++)
	{
		const UUpgrade * upgrade = AllUnits[teamid].Upgrades[i].UpgradeClass.GetDefaultObject();
		if (upgrade->CanUpgrade(Minion))
		{
			const uint32 upgraderank = AllUnits[teamid].Upgrades[i].Rank;
			
			const bool check = upgraderank > UPGRADE_UNLEARNED;
			checkf(check, TEXT("ARTFPSGameState::ApplyGlobalUpgrades : Upgrade Applied of Rank 0"));
			
			for (uint32 k = 0U; k < upgraderank; k++)
			{
				Minion->OnApplyUpgrade(upgrade);
			}

		}
	}
}

void ARTFPSGameState::ApplyPlayerUpgrades(ARTSMinion * PlayerPawn, AFPSPlayerState * InState) const
{
	if (InState != nullptr)
	{
		const TArray<TSubclassOf<UUpgrade>> upgrades = InState->GetAppliedUpgrades();

		for (int i = 0; i < upgrades.Num(); i++)
		{
			const uint32 upgraderank = InState->GetCurrentUpgradeRankFor(upgrades[i]);
			const bool check = upgraderank > UPGRADE_UNLEARNED;
			checkf(check, TEXT(" ARTFPSGameState::ApplyPlayerUpgrades : Upgrade Applied of Rank 0"));

			const UUpgrade * upgrade = upgrades[i].GetDefaultObject();
			
			for (uint32 k = 0U; k < upgraderank; k++)
			{
				PlayerPawn->OnApplyUpgrade(upgrade);
			}
		}
	}
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
			const int newval = *currentval + amount;
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
		const int* currentval = TeamResources[TeamID].Find(ResourceClass);
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

void ARTFPSGameState::UnpackUnitPriceMap(TMap<TSubclassOf<UObject>, FReplicationResourceMap> GameModePrices)
{
	for (const TPair<TSubclassOf<UObject>, FReplicationResourceMap>& pair : GameModePrices)
	{
		PurchasableUnits.Emplace(pair.Key);
		UnitCosts.Emplace(pair.Value);
	}
}

void ARTFPSGameState::AddRTSObjectToTeam(IRTSObjectInterface * const InObject)
{
	const int teamid = InObject->GetTeam();
	if(IsTeamValid(teamid))
	{
		ARTSMinion * isminion = Cast<ARTSMinion>(InObject); 
		ARTSStructure * isstructure = Cast<ARTSStructure>(InObject);
		URTSUpgrade * isupgrade = Cast<URTSUpgrade>(InObject);
		/**/
		if(isminion != nullptr)
		{ 
			AllUnits[teamid].Minions.AddUnique(isminion);
			for(int i = 0; i < Teams[teamid].Num(); i++)
			{
				/*Currently no reason to send clients Minions on thier team*/
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
		else if (isupgrade != nullptr  && isupgrade->IsGlobal())
		{
			const TSubclassOf<UUpgrade> upgradeclass = isupgrade->GetClass();
			
			FUpgradeInfo infocheck = FUpgradeInfo();
			infocheck.UpgradeClass = upgradeclass;
			infocheck.Rank = 1; 

			int upgradeindex = AllUnits[teamid].Upgrades.Find(infocheck);
			if(upgradeindex != INDEX_NONE)
			{
				/*If We've implemented this upgrade before, increase its rank*/
				AllUnits[teamid].Upgrades[upgradeindex].Rank++;
			}
			else
			{
				/*New Upgrade, Add it to the list*/
				AllUnits[teamid].Upgrades.AddUnique(infocheck);
			}
		}
	}
}

bool ARTFPSGameState::RemoveRTSObjectFromTeam(IRTSObjectInterface * InObject)
{
	bool retval = false;
	const int teamid = InObject->GetTeam();
	if(IsTeamValid(teamid))
	{
		ARTSMinion * isminion = Cast<ARTSMinion>(InObject); 
		ARTSStructure * isstructure = Cast<ARTSStructure>(InObject);

		/*check type casting and attempt to remove data*/
		if(isminion != nullptr && (AllUnits[teamid].Minions.Remove(isminion) >= (int32)1))
		{
			retval = true;
			for(int i = 0; i < Teams[teamid].Num(); i++)
			{
				/*Currently no reason to send clients Minions on thier team*/
			}
		}
		else if(isstructure != nullptr && (AllUnits[teamid].Structures.Remove(isstructure) >= (int32)1))
		{
			retval = true;
			for(int i = 0; i < Teams[teamid].Num(); i++)
			{
				/*For each player, update the list of Structures unique to thier team*/
				ARTFPSPlayerState * ps = Cast<ARTFPSPlayerState>(Teams[teamid][i]);
				check(ps);
				ps->SetTeamStructures(AllUnits[teamid].Structures);
			}
		}

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

int ARTFPSGameState::GetTeamResourceValue(int TeamID, TSubclassOf<AResource> ResourceClass) const
{
	int retval = -1;
	if (IsTeamValid(TeamID) && TeamResources[TeamID].Find(ResourceClass))
	{
		retval = *TeamResources[TeamID].Find(ResourceClass);
	}

	return(retval);
}

bool ARTFPSGameState::PurchaseUnit(TSubclassOf<UObject> PurchaseClass, ARTSPlayerController * Purchaser)
{
	bool retval = false;
	if (IsUnitPurchaseable(PurchaseClass, Purchaser))
	{
		const FReplicationResourceMap costs = GetUnitPrice(PurchaseClass);
		retval = RemoveTeamResource(Purchaser->GetTeamID(), costs.GetMap());
	}

	return(retval);

}

FReplicationResourceMap ARTFPSGameState::RefundUnit(TSubclassOf<UObject> RefundClass, ARTSPlayerController * Purchaser)
{
	return FReplicationResourceMap();
}

bool ARTFPSGameState::IsUnitPurchaseable(TSubclassOf<UObject> PurchaseClass, AController* Purchaser) const
{
	bool ispurchasable = PurchasableUnits.IndexOfByKey(PurchaseClass) != INDEX_NONE;
	ispurchasable &= (PurchasableUnits.Num() == UnitCosts.Num());

	return (ispurchasable);
}

FReplicationResourceMap ARTFPSGameState::GetUnitPrice(TSubclassOf<UObject> PurchaseClass) const
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

bool ARTFPSGameState::PurchaseExpUpgrade(const TSubclassOf<UUpgrade> PurchaseClass, IExpAccumulatorInterface * Purchaser, IUpgradableInterface * ToApply) const
{
	bool retval = true;
	const UUpgrade* defaultupgrade = PurchaseClass.GetDefaultObject();
	IUpgradableInterface * toupgrade = ToApply;
	checkf(defaultupgrade,TEXT("ARTFPSGameState::PurchaseExpUpgrade : Failed to Obtain Upgrade CDO"))
	
	const bool hasupgradepoints = Purchaser->GetAvailableUpgradePoints() > 0U;
	const bool canupgrade = defaultupgrade->CanUpgrade(ToApply);

	if (canupgrade && hasupgradepoints)
	{
		Purchaser->SpendUpgradePoints(1U);
		ToApply->OnApplyUpgrade(defaultupgrade);
	}
	else
	{
		retval = false;
		return retval;
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
	const int teamid = ps->GetTeamID();

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

void ARTFPSGameState::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	if (HasAuthority())
	{
		UpgradeManager = SpawnUpgradeManager();
	}
	else
	{
		RefreshAllUnits();
	}
}

void ARTFPSGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ARTFPSGameState, TeamResources);
	DOREPLIFETIME_CONDITION(ARTFPSGameState, MapResourceInfo, COND_InitialOnly);
	DOREPLIFETIME_CONDITION(ARTFPSGameState, UnitCosts, COND_InitialOnly);
	DOREPLIFETIME_CONDITION(ARTFPSGameState, PurchasableUnits, COND_InitialOnly);
}

AUpgradeManager * ARTFPSGameState::SpawnUpgradeManager()
{
	UWorld * world = GetWorld();
	check(world);

	AUpgradeManager * retval = world->SpawnActor<AUpgradeManager>(AUpgradeManager::StaticClass());

	return retval;
}

AUpgradeManager * ARTFPSGameState::GetUpgradeManager() const
{
	return UpgradeManager;
}





