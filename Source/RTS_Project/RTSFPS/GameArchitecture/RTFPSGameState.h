// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RTS_Project/TeamMultiplayerGame/Game/GameArchitecture/DefaultGameState.h"
#include "RTS_Project/RTSFPS/FPS/FPSServerController.h"
#include "RTS_Project/RTSFPS/FPS/FPSPlayerState.h"
#include "RTFPSMode.h"
#include "RTS_Project/RTSFPS/GameSystems/UpgradeSystem/UpgradeManager.h"
#include "RTS_Project/RTSFPS/GameSystems/UpgradeSystem/Interfaces/ExpAccumulatorInterface.h"
#include "RTS_Project/RTSFPS/GameSystems/ResourceSystem/Interfaces/ResourceVendorInterface.h"
#include "TeamResourceState.h"
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
class RTS_PROJECT_API ARTFPSGameState : public ADefaultGameState, public IResourceVendorInterface
{
	GENERATED_BODY()

	public:
		ARTFPSGameState(const FObjectInitializer & FOI);

		/*******************Unit Event Handling*****************/
		virtual void RefreshAllUnits();
		virtual void OnUnitDeath(IRTSObjectInterface* Unit);
		virtual void HandlePlayerDeath(AFPSServerController * Controller);
		/******************************************************/

	/********************IResourceVendorInterface**********************/
	public:
		virtual bool PurchaseUnit(const TSubclassOf<UObject> PurchaseClass, IResourceGatherer* Purchaser, AController* InstigatingController = nullptr) override;
		virtual FReplicationResourceMap RefundUnit(const TSubclassOf<UObject> RefundClass, IResourceGatherer* ToRefund, AController* InstigatingController = nullptr) override;
		virtual bool IsUnitPurchaseable(const TSubclassOf<UObject> PurchaseClass, IResourceGatherer* Purchaser, AController* InstigatingController = nullptr) const override;
		virtual FReplicationResourceMap GetDefaultUnitPrice(const TSubclassOf<UObject> PurchaseClass) const override;
	/***************************************************************/

		bool PurchaseExpUpgrade(const TSubclassOf<UUpgrade> PurchaseClass, IExpAccumulatorInterface* Purchaser, IUpgradableInterface* ToApply) const;


    public:
		virtual bool TeamInitialize(ADefaultMode* GameMode) override;
		virtual void PlayerGameDataInit(ADefaultPlayerState * Player) override;

	protected:
		virtual void InitializeUnitPriceMap(ARTFPSMode * GM);

	public:
		virtual void AddRTSObjectToTeam(IRTSObjectInterface * const InObject);
		virtual bool RemoveRTSObjectFromTeam(IRTSObjectInterface * InObject);

	public:
		virtual void RegisterRTSObject(IRTSObjectInterface* InObject);
		virtual void UnRegisterRTSObject(IRTSObjectInterface* InObject);
		virtual const TArray<IRTSObjectInterface*>& GetRegisteredRTSObjects() const;

    protected:
		virtual void PostInitializeComponents() override;
		virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
		virtual void ReceivedGameModeClass() override;
	
	/********************Upgrades**********************/
	AUpgradeManager * UpgradeManager = nullptr;
	AUpgradeManager * SpawnUpgradeManager();
	AUpgradeManager * GetUpgradeManager() const;
	/*************************************************/
	protected:
		UPROPERTY(Replicated)
		TArray<FResourceUIData> MapResourceInfo;
	

		RTSTeamUnits InvalidUnits;

	protected:
		TArray<IRTSObjectInterface*> RTSObjects;

		TMap<TSubclassOf<UObject>, FReplicationResourceMap> UnitBaseCosts;
};
