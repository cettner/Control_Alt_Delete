// Fill out your copyright notice in the Description page of Project Settings.

#include "TeamResourceState.h"
#include "RTS_Project/TeamMultiplayerGame/Game/GameArchitecture/DefaultPlayerController.h"
#include "RTFPSMode.h"

#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"


ATeamResourceState::ATeamResourceState() : Super()
{
}

void ATeamResourceState::LoadServerDefaults(ADefaultMode* InGameMode)
{
	const ARTFPSMode * gm = Cast<ARTFPSMode>(InGameMode);
	const FReplicationResourceMap startingresources = gm->GetStartingResources();
	AddResource(startingresources);
}

void ATeamResourceState::OnRep_TeamResources()
{
}

void ATeamResourceState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATeamResourceState, TeamResources);
	DOREPLIFETIME(ATeamResourceState, Structures);
	DOREPLIFETIME(ATeamResourceState, Minions);
	DOREPLIFETIME(ATeamResourceState, Upgrades);
}



void ATeamResourceState::AddRTSUnit(IRTSObjectInterface* InObject)
{
	if (ARTSMinion* isminion = Cast<ARTSMinion>(InObject))
	{
		AddRTSMinion(isminion);
	}
	else if (ARTSStructure * isstructure = Cast<ARTSStructure>(InObject))
	{
		AddRTSStructure(isstructure);
	}
	else if (URTSUpgrade * isupgrade = Cast<URTSUpgrade>(InObject))
	{
		AddRTSUpgrade(isupgrade);
	}

}

bool ATeamResourceState::RemoveRTSUnit(IRTSObjectInterface* InObject)
{
	bool retval = false;

	if (ARTSMinion* isminion = Cast<ARTSMinion>(InObject))
	{
		retval = Minions.Remove(isminion) > 0U;
	}
	else if (ARTSStructure* isstructure = Cast<ARTSStructure>(InObject))
	{
		retval = Structures.Remove(isstructure) > 0U;
	}
	else if (URTSUpgrade* isupgrade = Cast<URTSUpgrade>(InObject))
	{
		//TODO, Implement ability to remove upgrades
	    ///	retval = Upgrades.Remove(isupgrade) > 0U;
	}

	return retval;
}

TArray<ARTSMinion*> ATeamResourceState::GetMinions() const
{
	return Minions;
}

TArray<ARTSStructure*> ATeamResourceState::GetStructures() const
{
	return Structures;
}

void ATeamResourceState::AddRTSMinion(ARTSMinion* InMinion)
{
	Minions.AddUnique(InMinion);
}

void ATeamResourceState::AddRTSStructure(ARTSStructure* InStructure)
{
	Structures.AddUnique(InStructure);
}

void ATeamResourceState::AddRTSUpgrade(URTSUpgrade* InUpgrade)
{
	const TSubclassOf<UUpgrade> upgradeclass = InUpgrade->GetClass();

	FUpgradeInfo infocheck = FUpgradeInfo();
	infocheck.UpgradeClass = upgradeclass;
	infocheck.Rank = 1;

	int upgradeindex = Upgrades.Find(infocheck);
	if (upgradeindex != INDEX_NONE)
	{
		/*If We've implemented this upgrade before, increase its rank*/
		Upgrades[upgradeindex].Rank++;
	}
	else
	{
		/*New Upgrade, Add it to the list*/
		Upgrades.AddUnique(infocheck);
	}
}

void ATeamResourceState::SpawnStructureAtLocation(TSubclassOf<AActor> StructureClass, FTransform SpawnTransform, ADefaultPlayerController* InvokedController)
{
	UWorld* world = GetWorld();
	check(world);

	ARTSStructure* structure = world->SpawnActorDeferred<ARTSStructure>(StructureClass, SpawnTransform);

	check(structure);

	const int teamid = InvokedController->GetTeamID();
	structure->SetTeam(teamid);

	UGameplayStatics::FinishSpawningActor(structure, SpawnTransform);
	AddRTSUnit(structure);
}

void ATeamResourceState::ApplyPlayerUpgrades(ARTSMinion* PlayerPawn, AFPSPlayerState* InState) const
{
	if (InState != nullptr)
	{
		const TArray<TSubclassOf<UUpgrade>> upgrades = InState->GetAppliedUpgrades();

		for (int i = 0; i < upgrades.Num(); i++)
		{
			const uint32 upgraderank = InState->GetCurrentUpgradeRankFor(upgrades[i]);
			const bool check = upgraderank > UPGRADE_UNLEARNED;
			checkf(check, TEXT(" ARTFPSGameState::ApplyPlayerUpgrades : Upgrade Applied of Rank 0"));

			const UUpgrade* upgrade = upgrades[i].GetDefaultObject();

			for (uint32 k = 0U; k < upgraderank; k++)
			{
				PlayerPawn->OnApplyUpgrade(upgrade);
			}
		}
	}
}

bool ATeamResourceState::DispatchUpgrade_Validate(TSubclassOf<UUpgrade> UpgradeClass, const TArray<AActor*>& Applyto)
{
	return true;
}

void ATeamResourceState::DispatchUpgrade_Implementation(TSubclassOf<UUpgrade> UpgradeClass, const TArray<AActor*>& Applyto)
{
	if (HasAuthority())
	{
		UUpgrade* newupgrade = NewObject<UUpgrade>(UUpgrade::StaticClass(), UpgradeClass);
		for (int i = 0; i < Applyto.Num(); i++)
		{
			IUpgradableInterface* UpgradeMe = Cast<IUpgradableInterface>(Applyto[i]);
			UpgradeMe->OnApplyUpgrade(newupgrade);
		}
	}


}

bool ATeamResourceState::CheckAndDispatchUpgrade(TSubclassOf<UUpgrade> UpgradeClass, TArray<AActor*>& Applyto)
{
	if (!HasAuthority()) return false;

	for (int i = 0; i < Applyto.Num(); i++)
	{
		IUpgradableInterface* UpgradeMe = Cast<IUpgradableInterface>(Applyto[i]);
		if (UpgradeMe == nullptr)
		{
			Applyto.RemoveAt(i);
			UE_LOG(LogTemp, Warning, TEXT("WARNING AUpgradeManager::CheckAndDispatchUpgrade Actor Does not implement Upgradable interface, Removing it from RPC"));
		}
	}

	DispatchUpgrade(UpgradeClass, Applyto);

	return true;
}



bool ATeamResourceState::SpawnUnitFromStructure(ARTSStructure* SpawningStructure, const FStructureQueueData SpawnData)
{
	bool retval = false;
	UWorld* World = GetWorld();
	/*Check if the Object is a Minion, or an Upgrade*/
	if (SpawnData.SpawnClass.Get()->IsChildOf(ARTSMinion::StaticClass()))
	{
		SpawnMinionFromStructure(SpawningStructure, SpawnData);
		retval = true;
	}
	else if (SpawnData.SpawnClass.Get()->IsChildOf(URTSUpgrade::StaticClass()))
	{
		SpawnUpgradeFromStructure(SpawningStructure, SpawnData);
		retval = true;
	}

	return(retval);
}

void ATeamResourceState::SpawnMinionFromStructure(ARTSStructure* SpawningStructure, const FStructureQueueData SpawnData)
{
	UWorld* world = GetWorld();
	FVector minionextent;
	FVector minionorigin;

	ARTSMinion* Minion = world->SpawnActorDeferred<ARTSMinion>(SpawnData.SpawnClass, SpawningStructure->GetTransform(), nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding);
	Minion->GetActorBounds(true, minionorigin, minionextent);
	//TODO Check CollisionArea before spawn.
	const FTransform spawntransform = SpawningStructure->FindActorSpawnLocation(minionextent);

	AController* pc = SpawnData.RecieveingController;
	AFPSPlayerState* ps = nullptr;

	if (pc != nullptr)
	{
		ps = pc->GetPlayerState<AFPSPlayerState>();
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
	AddRTSUnit(Minion);
}

void ATeamResourceState::SpawnUpgradeFromStructure(ARTSStructure* SpawningStructure, const FStructureQueueData SpawnData)
{
	/*Handle A new Upgrade Purchase*/
	const int teamid = SpawningStructure->GetTeam();
	URTSUpgrade* const defaultupgrade = Cast<URTSUpgrade>(SpawnData.SpawnClass.GetDefaultObject());
	const TSubclassOf<UUpgrade> upgradeclass = TSubclassOf<UUpgrade>(defaultupgrade->GetClass());
	const AController* PC = SpawnData.RecieveingController;
	TArray<AActor*> minionactors = TArray<AActor*>();

	if (defaultupgrade->IsGlobal() && defaultupgrade->IsPersistent())
	{
		/*Upgrade All Existing Minions On the Map*/
		for (int i = 0; i < Minions.Num(); i++)
		{
			if (defaultupgrade->CanUpgrade(Minions[i]))
			{
				minionactors.Emplace(Minions[i]);
			}
		}

		CheckAndDispatchUpgrade(upgradeclass, minionactors);
		AddRTSUnit(defaultupgrade);
	}
	else if (!defaultupgrade->IsGlobal() && defaultupgrade->IsPersistent())
	{
		ARTSMinion* playerpawn = PC->GetPawn<ARTSMinion>();
		if (IsValid(playerpawn) && defaultupgrade->CanUpgrade(playerpawn))
		{
			minionactors.Emplace(playerpawn);
		}
		CheckAndDispatchUpgrade(upgradeclass, minionactors);
		AFPSPlayerState* ps = PC->GetPlayerState<AFPSPlayerState>();
		ps->AddUpgrade(upgradeclass);
	}
}

void ATeamResourceState::ApplyGlobalUpgrades(IUpgradableInterface* ToUpgrade) const
{
	for (int i = 0; i < Upgrades.Num(); i++)
	{
		const UUpgrade* upgrade = Upgrades[i].UpgradeClass.GetDefaultObject();
		if (upgrade->CanUpgrade(ToUpgrade))
		{
			const uint32 upgraderank = Upgrades[i].Rank;

			const bool check = upgraderank > UPGRADE_UNLEARNED;
			checkf(check, TEXT("ARTFPSGameState::ApplyGlobalUpgrades : Upgrade Applied of Rank 0"));

			for (uint32 k = 0U; k < upgraderank; k++)
			{
				ToUpgrade->OnApplyUpgrade(upgrade);
			}

		}
	}
}

void ATeamResourceState::AddResource(TSubclassOf<UResource> ResourceClass, int amount)
{
	const int* currentval = TeamResources.Find(ResourceClass);
	if (currentval != nullptr)
	{
		const int newval = *currentval + amount;
		TeamResources.Emplace(ResourceClass, newval);
		OnRep_TeamResources();
	}
	else
	{
		TeamResources.Emplace(ResourceClass, amount);
	}
}

bool ATeamResourceState::RemoveResource(const TSubclassOf<UResource> ResourceClass, int amount)
{
	bool retval = false;
	if (HasAuthority())
	{
		const int* currentval = TeamResources.Find(ResourceClass);
		if (currentval != nullptr && *currentval >= amount)
		{
			int newval = *currentval - amount;
			TeamResources.Emplace(ResourceClass, newval);
			retval = true;
			OnRep_TeamResources();
		}
	}

	return retval;
}

void ATeamResourceState::AddResource(const FReplicationResourceMap InResourceMap)
{
	IResourceGatherer::AddResource(InResourceMap);
}

bool ATeamResourceState::RemoveResource(const FReplicationResourceMap InResourceMap)
{
	return IResourceGatherer::RemoveResource(InResourceMap);
}

bool ATeamResourceState::HasResource(const TSubclassOf<UResource> ResourceClass, const uint32 amount) const
{
	return IResourceGatherer::HasResource(ResourceClass, amount);
}

bool ATeamResourceState::HasResource(const FReplicationResourceMap InResourceMap) const
{
	return IResourceGatherer::HasResource(InResourceMap);
}

bool ATeamResourceState::CanCarryMore(TSubclassOf<UResource> ResourceClass, uint32 InNumtoCarry) const
{
	return true;
}

FReplicationResourceMap ATeamResourceState::GetAllHeldResources() const
{
	return TeamResources;
}

bool ATeamResourceState::GetHeldResource(TSubclassOf<UResource> ResourceClass, uint32& OutAmount) const
{
	return IResourceGatherer::GetHeldResource(ResourceClass, OutAmount);
}

uint32 ATeamResourceState::GetCurrentWeight() const
{
	return 0U;
}

uint32 ATeamResourceState::GetMaxWeight() const
{
	return 0xFFFFFFFF;
}
