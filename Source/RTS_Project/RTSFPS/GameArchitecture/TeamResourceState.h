// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RTS_Project/TeamMultiplayerGame/Game/GameArchitecture/TeamState.h"
#include "../GameSystems/ResourceSystem/Interfaces/ResourceGatherer.h"
#include "../Shared/Interfaces/RTSObjectInterface.h"
#include "../RTS/RTSMinion.h"
#include "../RTS/Structures/RTSStructure.h"
#include "../Shared/Upgrades/RTSUpgrade.h"

/*Todo, remove this depedency from upgrade implementation*/
#include "RTS_Project/RTSFPS/FPS/FPSPlayerState.h"

#include "TeamResourceState.generated.h"


UCLASS()
class RTS_PROJECT_API ATeamResourceState : public ATeamState, public IResourceGatherer
{
	GENERATED_BODY()
	
	friend class ARTSStructure;

	protected:
		ATeamResourceState();

	public:
		virtual void AddRTSUnit(IRTSObjectInterface* InObject);
		virtual bool RemoveRTSUnit(IRTSObjectInterface* InObject);
	
	public:
		TArray<ARTSMinion*> GetMinions() const;
		TArray<ARTSStructure*> GetStructures() const;

	protected:
		virtual void AddRTSMinion(ARTSMinion * InMinion);
		virtual void AddRTSStructure(ARTSStructure * InStructure);
		virtual void AddRTSUpgrade(URTSUpgrade* InUpgrade);

	protected:
		virtual void SpawnStructureAtLocation(TSubclassOf<AActor> StructureClass, FTransform SpawnTransform, ADefaultPlayerController* InvokedController = nullptr);

	protected:
		virtual bool SpawnUnitFromStructure(ARTSStructure* SpawningStructure, const FStructureQueueData SpawnData);
		virtual void SpawnMinionFromStructure(ARTSStructure* SpawningStructure, const FStructureQueueData SpawnData);
		virtual void SpawnUpgradeFromStructure(ARTSStructure* SpawningStructure, const FStructureQueueData SpawnData);
	
	protected:
		virtual void ApplyGlobalUpgrades(IUpgradableInterface* ToUpgrade) const;
		virtual void ApplyPlayerUpgrades(ARTSMinion* PlayerPawn, AFPSPlayerState* InController) const;

		UFUNCTION(NetMultiCast, reliable, WithValidation)
		void DispatchUpgrade(TSubclassOf<UUpgrade> UpgradeClass, const TArray<AActor*>& Applyto);
		bool CheckAndDispatchUpgrade(TSubclassOf<UUpgrade> UpgradeClass, TArray<AActor*>& Applyto);
	/*****************************IResourceGatherInterface******************************/
	public:
		virtual void AddResource(TSubclassOf<AResource> ResourceClass, int amount) override;
		virtual bool RemoveResource(const TSubclassOf<AResource> ResourceClass, int amount) override;
		virtual void AddResource(const FReplicationResourceMap InResourceMap) override;
		virtual bool RemoveResource(const FReplicationResourceMap InResourceMap) override;

		virtual bool HasResource(const TSubclassOf<AResource> ResourceClass, const uint32 amount = 0U) const override;
		virtual bool HasResource(const FReplicationResourceMap InResourceMap) const override;

		/*Returns the count of the specified class that the gatherer can carry until the cap is achieved.*/
		virtual uint32 CanCarryMore(TSubclassOf<AResource> ResourceClass) const override;

		/*Returns a Map of All resources Carried by the instance*/
		virtual FReplicationResourceMap GetAllHeldResources() const override;

		/*Returns the amount held by the unit of the particular type of Resource*/
		virtual uint32 GetHeldResource(TSubclassOf<AResource> ResourceClass) const override;
		virtual uint32 GetCurrentWeight() const override;
		virtual uint32 GetMaxWeight() const override;
	/************************************************************************************/

	protected:
		virtual void LoadServerDefaults(ADefaultMode* InGameMode) override;

		UFUNCTION()
		virtual void OnRep_TeamResources();

	protected:
		virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	protected:
		UPROPERTY(ReplicatedUsing = OnRep_TeamResources)
		FReplicationResourceMap TeamResources = FReplicationResourceMap();

	protected:
		UPROPERTY(Replicated)
		TArray<ARTSMinion*> Minions = TArray<ARTSMinion*>();
		
		UPROPERTY(Replicated)
		TArray<ARTSStructure*> Structures = TArray<ARTSStructure*>();
		
		UPROPERTY(Replicated)
		TArray<FUpgradeInfo> Upgrades = TArray<FUpgradeInfo>();
};
