// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RTS_Project/TeamMultiplayerGame/Game/GameArchitecture/DefaultGameState.h"
#include "RTS_Project/RTSFPS/FPS/FPSServerController.h"

#include "RTFPSMode.h"
#include "RTS_Project/RTSFPS/GameSystems/UpgradeSystem/UpgradeManager.h"
#include "RTS_Project/RTSFPS/GameSystems/UpgradeSystem/Interfaces/ExpAccumulatorInterface.h"
#include "RTS_Project/RTSFPS/GameSystems/ResourceSystem/Interfaces/ResourceVendorInterface.h"
#include "RTS_Project/RTSFPS/RTS/Orders/RTSResourcePurchaseOrder.h"
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
		virtual TMap<TSubclassOf<UObject>, FReplicationResourceMap> GetAllDefaultUnitPrices() const override;
	/***************************************************************/

		bool PurchaseExpUpgrade(const TSubclassOf<UUpgrade> PurchaseClass, IExpAccumulatorInterface* Purchaser, IUpgradableInterface* ToApply) const;


    protected:
		virtual bool TeamInitialize(ADefaultMode* GameMode) override;
		virtual void PlayerGameDataInit(ADefaultPlayerState * Player) override;

	/***********************Purchase Orders****************************/
	protected:
		virtual void SetUnitPriceMap(const TMap<TSubclassOf<UObject>, FReplicationResourceMap> InPriceMap);

	public:
		virtual TArray<URTSResourcePurchaseOrder*> GetPurchaseOrders(const TArray<TSubclassOf<UObject>> InPurchaseObjects) const;
	/********************************************************************/

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
		virtual bool ReplicateSubobjects(UActorChannel* Channel, FOutBunch* Bunch, FReplicationFlags* RepFlags) override;

	protected:
		UFUNCTION()
		virtual void OnRep_PurchaseOrdersRep();

	protected:
		UPROPERTY(ReplicatedUsing = OnRep_PurchaseOrdersRep)
		TArray<URTSResourcePurchaseOrder*> PurchaseOrdersRep;

	protected:
		TArray<IRTSObjectInterface*> RTSObjects;

		/*Any unit that wishes to have a purchase order property references from here, this is to reduce per-instance creation of properties*/
		TMap<TSubclassOf<UObject>,URTSResourcePurchaseOrder*> PurchaseOrders = TMap<TSubclassOf<UObject>, URTSResourcePurchaseOrder*>();
};
