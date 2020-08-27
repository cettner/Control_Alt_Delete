// Fill out your copyright notice in the Description page of Project Settings.

#include "RTFPSGameState.h"
#include "RTFPSPlayerState.h"
#include "RTS_Project/RTSFPS/FPS/Death/RespawnSelectionPawn.h"

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
			if (PS && PS->isRtsPlayer)
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

void ARTFPSGameState::OnMinionDeath(ARTSMinion* Minion)
{
	if (Minion && IsTeamValid(Minion->GetTeam()))
	{
		if (AllUnits[Minion->GetTeam()].Minions.Remove(Minion) != (int32)1)
		{
			UE_LOG(LogTemp, Warning, TEXT("[ARTFPSGameState::OnMinionDeath] Minion Removal Unsuccessful!"));
		}

		AFPSServerController* PC = Minion->GetController<AFPSServerController>();
		if (PC)
		{
			HandlePlayerDeath(PC);
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

	AFPSServerController* PC = Cast<AFPSServerController>(SpawnData.RecieveingController);

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
		int* currentval = TeamResources[TeamID].ValueMap.Find(ResourceClass);
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
		int* currentval = TeamResources[TeamID].ValueMap.Find(ResourceClass);
		if (currentval != nullptr && *currentval <= requestedamount)
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
		int* currentval = TeamResources[TeamID].ValueMap.Find(ResourceClass);
		if (currentval != nullptr && *currentval <= amount)
		{
			*currentval -= amount;
			retval = true;
		}
	}
	return retval;
}

int ARTFPSGameState::GetTeamResourceValue(int TeamID, TSubclassOf<AResource> ResourceClass)
{
	int retval = -1;
	if (IsTeamValid(TeamID) && TeamResources[TeamID].ValueMap.Find(ResourceClass))
	{
		retval = *TeamResources[TeamID].ValueMap.Find(ResourceClass);
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

			FResourceData Resources;

			for (int k = 0; k < MapResources.Num(); k++)
			{
				int startingval = Game->GetStartingResources(MapResources[k]);
				Resources.ValueMap.Emplace(MapResources[k], startingval);
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





