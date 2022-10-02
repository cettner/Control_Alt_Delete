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
	RTSObjects = TArray<IRTSObjectInterface*>();
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

void ARTFPSGameState::InitializeUnitPriceMap(ARTFPSMode * GameMode)
{
	/*Set for Server, Cleints load from the GameMode CDO on GM class replication*/
	UnitBaseCosts = GameMode->GetDefaultUnitCosts();
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

bool ARTFPSGameState::PurchaseUnit(const TSubclassOf<UObject> PurchaseClass, IResourceGatherer* Purchaser, AController* InstigatingController)
{
	bool retval = false;
	if (IsUnitPurchaseable(PurchaseClass, Purchaser))
	{
		const FReplicationResourceMap costs = GetUnitPriceForSource(PurchaseClass, Purchaser, InstigatingController);
		retval = Purchaser->RemoveResource(costs);
	}

	return(retval);

}

FReplicationResourceMap ARTFPSGameState::RefundUnit(const TSubclassOf<UObject> RefundClass, IResourceGatherer* ToRefund, AController* InstigatingController)
{
	return FReplicationResourceMap();
}

bool ARTFPSGameState::IsUnitPurchaseable(const TSubclassOf<UObject> PurchaseClass, IResourceGatherer* Purchaser, AController* InstigatingController) const
{
	const bool ispurchasable = UnitBaseCosts.Contains(PurchaseClass);
	return (ispurchasable);
}

FReplicationResourceMap ARTFPSGameState::GetDefaultUnitPrice(const TSubclassOf<UObject> PurchaseClass) const
{
	const FReplicationResourceMap * findmap = UnitBaseCosts.Find(PurchaseClass);
	FReplicationResourceMap retval = FReplicationResourceMap();

	if (findmap != nullptr)
	{
		retval = *findmap;
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

bool ARTFPSGameState::TeamInitialize(ADefaultMode* GameMode)
{
	bool result = Super::TeamInitialize(GameMode);

	ARTFPSMode * Game = Cast<ARTFPSMode>(GameMode);
	
	/*Give Each Player their Starting Resources and Unit Pricing*/
	InitializeUnitPriceMap(Game);

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
	DOREPLIFETIME_CONDITION(ARTFPSGameState, MapResourceInfo, COND_InitialOnly);
}

void ARTFPSGameState::ReceivedGameModeClass()
{
	const ARTFPSMode* gmcdo = Cast<ARTFPSMode>(GameModeClass.GetDefaultObject());
	UnitBaseCosts = gmcdo->GetDefaultUnitCosts();

	Super::ReceivedGameModeClass();
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





