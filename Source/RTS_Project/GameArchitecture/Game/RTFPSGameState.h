// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "DefaultGameState.h"
#include "RTS_Project/RTSFPS/FPS/FPSServerController.h"
#include "RTFPSMode.h"
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
};


UCLASS()
class RTS_PROJECT_API ARTFPSGameState : public ADefaultGameState
{
	GENERATED_BODY()

	public:
		ARTFPSGameState(const FObjectInitializer & FOI);
		int NumRTSPlayers(int Team_Index);
		virtual void RefreshAllUnits();
		virtual void OnUnitDeath(IRTSObjectInterface* Unit);
		virtual void HandlePlayerDeath(AFPSServerController * Controller);
		virtual void HandleStructureMinionSpawn(ARTSStructure* SpawningStructure, FStructureQueueData SpawnData);
		virtual void HandleStructureSpawn(TSubclassOf<AActor> StructureClass, FTransform SpawnTransform, ADefaultPlayerController * InvokedController = nullptr);

		virtual TArray<ARTSMinion *> GetAllMinionsOfTeam(int teamindex) const;
		virtual TArray<ARTSStructure *> GetAllStructuresOfTeam(int teamindex) const;
		
		virtual TArray<TSubclassOf<AResource>> GetResourceTypes() const;
		virtual TArray<FResourceUIData> GetMapResourceInfo() const;
		virtual bool IsTeamResourceAvailable(int TeamID, TSubclassOf<AResource> ResourceClass, int requestedamount) const;
		virtual bool IsTeamResourceAvailable(int TeamID, FReplicationResourceMap requestedamount) const;

		virtual int GetTeamResourceValue(int TeamID, TSubclassOf<AResource> ResourceClass) const;

		virtual bool PurchaseUnit(TSubclassOf<AActor> PurchaseClass, ARTSPlayerController* Purchaser);
		virtual FReplicationResourceMap RefundUnit(TSubclassOf<AActor> RefundClass, ARTSPlayerController* Purchaser = nullptr);
		virtual bool IsUnitPurchaseable(TSubclassOf<AActor> PurchaseClass, AController * Purchaser = nullptr) const;
		virtual FReplicationResourceMap GetUnitPrice(TSubclassOf<AActor> PurchaseClass) const;
		virtual bool ScoreResource(TSubclassOf<AResource> ResourceType, int Amount, IRTSObjectInterface* Donar);

    public:
		virtual bool TeamInitialize(ADefaultMode* GameMode) override;
		virtual void PlayerGameDataInit(APlayerState * Player) override;

	protected:
		virtual void InitializeResources(ARTFPSMode * GM);
		virtual bool AddTeamResource(int TeamID, TSubclassOf<AResource> ResourceClass, int amount);
		virtual bool RemoveTeamResource(int TeamID, TSubclassOf<AResource> ResourceClass, int amount);
		virtual bool RemoveTeamResource(int TeamID, TMap<TSubclassOf<AResource>, int> ResourceCosts);
		virtual void UnpackUnitPriceMap(TMap<TSubclassOf<AActor>, FReplicationResourceMap> GameModePrices);
	
	public:
		virtual void AddRTSObjectToTeam(IRTSObjectInterface * InObject);
		virtual bool RemoveRTSObjectFromTeam(IRTSObjectInterface * InObject);


    protected:
		virtual bool InitializeMapResourceInfo(TArray<TSubclassOf<AResource>> ResourceClasses);

    protected:
		virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	protected:
		UPROPERTY(Replicated)
		TArray<FReplicationResourceMap> TeamResources;

		UPROPERTY(Replicated)
		TArray<TSubclassOf<AActor>> PurchasableUnits;

		UPROPERTY(Replicated)
		TArray<FReplicationResourceMap> UnitCosts;

		UPROPERTY(Replicated)
		TArray<FResourceUIData> MapResourceInfo;

		private:
		TArray<RTSTeamUnits> AllUnits;

		RTSTeamUnits InvalidUnits;
};
