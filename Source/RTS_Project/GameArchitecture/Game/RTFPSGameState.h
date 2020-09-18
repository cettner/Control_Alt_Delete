// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DefaultGameState.h"
#include "RTS_Project/RTSFPS/FPS/FPSServerController.h"
#include "RTFPSMode.h"
#include "RTFPSGameState.generated.h"


/*Foward Declarations*/
class  ARTSMinion;
class  ARTSStructure;
struct FStructureQueueData;

struct RTSTeamUnits
{
	TArray<ARTSMinion*> Minions = TArray<ARTSMinion*>();
	TArray<ARTSStructure*> Structures = TArray<ARTSStructure*>();
};

USTRUCT()
struct FReplicationResourceMap
{
	GENERATED_USTRUCT_BODY()
public: 
	void Emplace(TSubclassOf<AResource> Key, int Value)
	{
		int index = Keys.IndexOfByKey(Key);

		if (index == INDEX_NONE)
		{
			Keys.Emplace(Key);
			Values.Emplace(Value);
		}
		else
		{
			Values[index] = Value;
		}
	}
	int* Find(TSubclassOf<AResource> Key)
	{
		int index = Keys.IndexOfByKey(Key);
		int* retval = nullptr;

		if (index != INDEX_NONE)
		{
			retval = &Values[index];
		}

		return(retval);
	}
	int Num() const
	{
		return(Keys.Num());
	}
	TMap<TSubclassOf<AResource>, int> GetMap() const
	{
		TMap<TSubclassOf<AResource>, int> Map = TMap<TSubclassOf<AResource>, int>();
		if (IsValid())
		{
			for (int i = 0; i < Keys.Num(); i++)
			{
				Map.Emplace(Keys[i], Values[i]);
			}
		}

		
		return(Map);
	}
	bool IsValid() const
	{
		return(Keys.Num() == Values.Num());
	}

protected:
	UPROPERTY(EditDefaultsOnly)
	TArray<int> Values;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<AResource>> Keys;
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
		virtual bool RemoveTeamResource(int TeamID, TMap<TSubclassOf<AResource>, int> ResourceCosts);
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
		TArray<FReplicationResourceMap> TeamResources;

		UPROPERTY(EditAnywhere)
		TArray<FResourceUIData> MapResourceInfo;
};
