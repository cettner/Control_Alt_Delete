// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DefaultGameState.h"
#include "RTS_Project/RTSFPS/BaseClasses/RTSMinion.h"
#include "RTS_Project/RTSFPS/RTS/Structures/RTSStructure.h"
#include "RTS_Project/RTSFPS/FPS/FPSServerController.h"
#include "RTFPSMode.h"
#include "RTFPSGameState.generated.h"

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
		virtual void OnMinionDeath(ARTSMinion* Minion);
		virtual void HandlePlayerDeath(AFPSServerController * Controller);
		virtual void HandleStructureMinionSpawn(ARTSStructure* SpawningStructure, FStructureQueueData SpawnData);
		virtual TArray<ARTSMinion *> GetAllMinionsOfTeam(int teamindex) const;
		virtual TArray<ARTSStructure *> GetAllStructuresOfTeam(int teamindex) const;
		
		virtual TArray<TSubclassOf<AResource>> GetResourceTypes() const;
		virtual TArray<FResourceUIData> GetMapResourceInfo() const;

		virtual bool AddTeamResource(int TeamID, TSubclassOf<AResource> ResourceClass, int amount);
		virtual bool IsTeamResourceAvailable(int TeamID, TSubclassOf<AResource> ResourceClass, int requestedamount);
		virtual bool RemoveTeamResource(int TeamID, TSubclassOf<AResource> ResourceClass, int amount);
		virtual int GetTeamResourceValue(int TeamID, TSubclassOf<AResource> ResourceClass);

    public:
		virtual bool TeamInitialize(ADefaultMode* GameMode) override;

    protected:
		virtual bool InitializeMapResourceInfo(TArray<TSubclassOf<AResource>> ResourceClasses);

    protected:
		virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	protected:
		TArray<RTSTeamUnits> AllUnits;
		RTSTeamUnits InvalidUnits;

		UPROPERTY(Replicated)
		TArray<FResourceData> TeamResources;

		UPROPERTY(EditAnywhere)
		TArray<FResourceUIData> MapResourceInfo;
};
