// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DefaultGameState.h"
#include "RTS_Project/RTSFPS/FPS/FPSServerController.h"
#include "RTS_Project/RTSFPS/FPS/FPSPlayerState.h"
#include "RTFPSMode.h"
#include "RTS_Project/RTSFPS/GameSystems/UpgradeSystem/UpgradeManager.h"
#include "RTS_Project/RTSFPS/GameSystems/UpgradeSystem/Interfaces/ExpAccumulatorInterface.h"
#include "RTFPSGameState.generated.h"



#define SELECTION_CHANNEL  ECC_GameTraceChannel1
#define WEAPON_CHANNEL ECC_GameTraceChannel2
#define GRID_CHANNEL ECC_GameTraceChannel3

/*Foward Declarations*/
class  ARTSMinion;
class  ARTSStructure;
class IRTSObjectInterface;
struct FStructureQueueData;

struct RTSTeamUnits
{
	TArray<ARTSMinion*> Minions = TArray<ARTSMinion*>();
	TArray<ARTSStructure*> Structures = TArray<ARTSStructure*>();
	TArray<FUpgradeInfo> Upgrades = TArray<FUpgradeInfo>();
};


UCLASS()
class RTS_PROJECT_API ARTFPSGameState : public ADefaultGameState
{
	GENERATED_BODY()

	public:
		ARTFPSGameState(const FObjectInitializer & FOI);
		int NumRTSPlayers(int Team_Index);
		/*******************Unit Event Handling*****************/
		virtual void RefreshAllUnits();
		virtual void OnUnitDeath(IRTSObjectInterface* Unit);
		virtual void HandlePlayerDeath(AFPSServerController * Controller);
		virtual void SpawnObjectFromStructure(ARTSStructure* SpawningStructure, FStructureQueueData SpawnData);
		virtual void HandleStructureSpawn(TSubclassOf<AActor> StructureClass, FTransform SpawnTransform, ADefaultPlayerController * InvokedController = nullptr);
		virtual void ApplyGlobalUpgrades(ARTSMinion * Minion) const;
		virtual void ApplyPlayerUpgrades(ARTSMinion * PlayerPawn, AFPSPlayerState * InController) const;
		/******************************************************/

		virtual TArray<ARTSMinion *> GetAllMinionsOfTeam(int teamindex) const;
		virtual TArray<ARTSStructure *> GetAllStructuresOfTeam(int teamindex) const;
		
		virtual TArray<TSubclassOf<AResource>> GetResourceTypes() const;
		virtual TArray<FResourceUIData> GetMapResourceInfo() const;
		virtual bool IsTeamResourceAvailable(int TeamID, TSubclassOf<AResource> ResourceClass, int requestedamount) const;
		virtual bool IsTeamResourceAvailable(int TeamID, FReplicationResourceMap requestedamount) const;
		virtual int GetTeamResourceValue(int TeamID, TSubclassOf<AResource> ResourceClass) const;
		virtual bool ScoreResource(TSubclassOf<AResource> ResourceType, int Amount, IRTSObjectInterface* Donar);

	/********************Purchasing**********************/
	public:
		virtual bool PurchaseUnit(TSubclassOf<UObject> PurchaseClass, ARTSPlayerController* Purchaser);
		virtual FReplicationResourceMap RefundUnit(TSubclassOf<UObject> RefundClass, ARTSPlayerController* Purchaser = nullptr);
		virtual bool IsUnitPurchaseable(TSubclassOf<UObject> PurchaseClass, AController * Purchaser = nullptr) const;
		virtual FReplicationResourceMap GetUnitPrice(TSubclassOf<UObject> PurchaseClass) const;
		virtual bool PurchaseExpUpgrade(const TSubclassOf<UUpgrade> PurchaseClass, IExpAccumulatorInterface * Purchaser, IUpgradableInterface * ToApply) const;

	protected:
		virtual void UnpackUnitPriceMap(TMap<TSubclassOf<UObject>, FReplicationResourceMap> GameModePrices);
	/*************************************************/



    public:
		virtual bool TeamInitialize(ADefaultMode* GameMode) override;
		virtual void PlayerGameDataInit(APlayerState * Player) override;

	protected:
		virtual void InitializeResources(ARTFPSMode * GM);
		virtual bool AddTeamResource(int TeamID, TSubclassOf<AResource> ResourceClass, int amount);
		virtual bool RemoveTeamResource(int TeamID, TSubclassOf<AResource> ResourceClass, int amount);
		virtual bool RemoveTeamResource(int TeamID, TMap<TSubclassOf<AResource>, int> ResourceCosts);

	public:
		virtual void AddRTSObjectToTeam(IRTSObjectInterface * const InObject);
		virtual bool RemoveRTSObjectFromTeam(IRTSObjectInterface * InObject);


    protected:
		virtual bool InitializeMapResourceInfo(TArray<TSubclassOf<AResource>> ResourceClasses);

    protected:
		virtual void PostInitializeComponents() override;
		virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	
	/********************Upgrades**********************/
	AUpgradeManager * UpgradeManager = nullptr;
	AUpgradeManager * SpawnUpgradeManager();
	AUpgradeManager * GetUpgradeManager() const;
	/*************************************************/
	protected:
		UPROPERTY(Replicated)
		TArray<FReplicationResourceMap> TeamResources;

		UPROPERTY(Replicated)
		TArray<TSubclassOf<UObject>> PurchasableUnits;

		UPROPERTY(Replicated)
		TArray<FReplicationResourceMap> UnitCosts;

		UPROPERTY(Replicated)
		TArray<FResourceUIData> MapResourceInfo;

		private:
		TArray<RTSTeamUnits> AllUnits;

		RTSTeamUnits InvalidUnits;
};
