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
	for (TObjectIterator<ARTSMinion> Itr; Itr; ++Itr)
	{
		ARTSMinion* minion = *Itr;
		if (minion && IsTeamValid(minion->GetTeam()))
		{
			AllUnits[minion->GetTeam()].Minions.AddUnique(minion);
		}
		else
		{
			InvalidUnits.Minions.Add(minion);
		}
	}

	for (TObjectIterator<ARTSStructure> Itr; Itr; ++Itr)
	{
		ARTSStructure* structure = *Itr;
		if (structure && IsTeamValid(structure->GetTeam()))
		{
			AllUnits[structure->GetTeam()].Structures.AddUnique(structure);
		}
		else
		{
			InvalidUnits.Structures.Add(structure);
		}
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


	ARespawnSelectionPawn * Respawnactor =  World->SpawnActor<ARespawnSelectionPawn>(ARespawnSelectionPawn::StaticClass(),RespawnActorParams);
	if (structures.Num() && Respawnactor)
	{
		Respawnactor->SetRevolveActor(structures[0]);
	}
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

bool ARTFPSGameState::AddTeamResource(int TeamID, TSubclassOf<AResource> ResourceClass, int amount)
{
	bool retval = false;
	if (IsTeamValid(TeamID))
	{
		int* currentval = TeamResources[TeamID].Find(ResourceClass);
		if (currentval != nullptr)
		{
			*currentval += amount;
			retval = true;
		}
	}
	return retval;
}

bool ARTFPSGameState::IsTeamResourceAvailable(int TeamID, TSubclassOf<AResource> ResourceClass, int requestedamount)
{
	bool retval = false;
	if (IsTeamValid(TeamID))
	{
		int* currentval = TeamResources[TeamID].Find(ResourceClass);
		if (currentval != nullptr && *currentval >= requestedamount)
		{
			retval = true;
		}
	}
	return retval;
}

bool ARTFPSGameState::RemoveTeamResource(int TeamID, TSubclassOf<AResource> ResourceClass, int amount)
{
	bool retval = false;
	if (IsTeamValid(TeamID))
	{
		int* currentval = TeamResources[TeamID].Find(ResourceClass);
		if (currentval != nullptr && *currentval >= amount)
		{
			*currentval -= amount;
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

int ARTFPSGameState::GetTeamResourceValue(int TeamID, TSubclassOf<AResource> ResourceClass)
{
	int retval = -1;
	if (IsTeamValid(TeamID) && TeamResources[TeamID].Find(ResourceClass))
	{
		retval = *TeamResources[TeamID].Find(ResourceClass);
	}

	return(retval);
}

bool ARTFPSGameState::TeamInitialize(ADefaultMode* GameMode)
{
	bool result = Super::TeamInitialize(GameMode);

	ARTFPSMode * Game = Cast<ARTFPSMode>(GameMode);
	
	if (result)
	{
		TArray<TSubclassOf<AResource>> MapResources = GetResourceTypes();

		InitializeMapResourceInfo(MapResources);

		for (int i = 0; i < GameMode->GetNumTeams(); i++)
		{
			RTSTeamUnits newunitteam;
			AllUnits.Emplace(newunitteam);

			FReplicationResourceMap Resources;

			for (int k = 0; k < MapResources.Num(); k++)
			{
				int startingval = Game->GetStartingResources(MapResources[k]);
				Resources.Emplace(MapResources[k], startingval);
				Game->GetStartingResources(MapResources[k]);
			}
			TeamResources.Emplace(Resources);
		}
	}

	RefreshAllUnits();

	return result;
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
}





