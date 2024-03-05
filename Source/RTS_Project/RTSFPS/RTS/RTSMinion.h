// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "BehaviorTree/BehaviorTree.h"

#include "RTS_Project/RTSFPS/GameSystems/UpgradeSystem/UpgradeData.h"
#include "RTS_Project/RTSFPS/Shared/Interfaces/RTSObjectInterface.h"
#include "RTS_Project/RTSFPS/Shared/Interfaces/CombatInterface.h"
#include "RTS_Project/RTSFPS/GameSystems/HealthSystem/DeathComponent.h"
#include "RTS_Project/RTSFPS/GameSystems/ResourceSystem/ResourceGathererComponent.h"
#include "RTS_Project/RTSFPS/GameSystems/ResourceSystem/ResourceTypes/HealthResource.h"
#include "RTS_Project/RTSFPS/Shared/Components/DecalSelectionComponent.h"
#include "RTS_Project/RTSFPS/RTS/Minions/AI/RTSAIController.h"
#include "./Orders/RTSMoveOrder.h"
#include "RTSMinion.generated.h"


/*Forward Declarations*/
class ACommander;

UCLASS(Blueprintable)
class ARTSMinion : public ACharacter, public IRTSObjectInterface, public ICombatInterface, public IResourceGatherer
{
	GENERATED_BODY()

public:
	ARTSMinion();

	FORCEINLINE TSubclassOf<UUpgradeData> GetUpgradeDataClass() const { return UpgradeDataClass; }

	virtual TSubclassOf<UResource> GetResourceForDamageEvent(const TSubclassOf<UDamageType>& InDamageType) const;

	virtual bool IsEnemy(const AActor* InMinion) const;

	virtual bool IsEnemy(const IRTSObjectInterface* InRTSObject) const;

	virtual void ClearCommander();

	virtual void SetCommander(ACommander* Commander);

protected:
	UFUNCTION()
	virtual void OnRep_TeamID();
	
	virtual void OnHealthResourceChanged(const TSubclassOf<UResource> InResourceClass, const uint32 InOldValue, const uint32 InNewValue, TScriptInterface<IResourceGatherer> InGatherer);
	
	virtual void OnDeath();
	
	UFUNCTION()
	virtual void OnUpgradeChanged(const TSubclassOf<UUpgrade> Upgradeclass, const int32 OldRank, const int32 NewRank);
	virtual bool InstallUpgrade(const TSubclassOf<UUpgrade> Upgradeclass, const int32 OldRank, const int32 NewRank);
	virtual UObject* GetUpgradeSubObject(const TSubclassOf<UUpgrade>& InUpgradeclass) const;



private:
	UFUNCTION()
	void OnNotifyReadyforRegistration(AGameStateBase* GameState);

	/********************************CombatInterface************************************/
public:
	virtual bool StartAttack(const int32 InAttackID = -1) override;

	virtual bool StopAttack(const bool InForceStop = false) override;

	virtual int32 GetAttackIndexForTarget(const AActor * InToAttack) const override;
	/************************************************************************************/


/********************AI**************************/
public:
	UBehaviorTree* GetBehavior();

	FRTSAIPerceptionConfig GetAIConfig() const;

	bool UsesAISenses() const;

	TArray<TSubclassOf<UAISense>> GetAISenses() const;
/***********************************************/

/**************IRTSObjectInterface****************/
public:
	virtual void SetSelected()  override;
	virtual void SetDeselected() override;
	virtual void SetTeam(int team_id) override;
	virtual int GetTeam() const override;
	virtual void OnLocalPlayerTeamChange(int InLocalTeamID) override;
	virtual bool IsLocalEnemy() const override;
	virtual IRTSObjectInterface * GetLeadRTSObject() override;
	virtual void SetTeamColors(FLinearColor TeamColor) override; 
	virtual FOnUnitDeathDelegate& GetUnitDeathDelegate() override;
	virtual bool IsAlive() const override;
	virtual bool IsBoxSelectable() const override;
	virtual UTexture* GetThumbnail(const UUserWidget* InDisplayContext = nullptr) const override;
	virtual FName GetUnitName() const override;
	virtual const TSubclassOf<URTSTargetedOrder> GetDefaultOrderClass(const FOrderContext& InHitContext) const override;
	virtual void IssueOrder(AController* Issuer, const FOrderContext& InHitContext, URTSOrder* InOrder = nullptr, const bool InbIsQueuedOrder = false) override;
	virtual URTSOrder* GetCurrentOrder() const override;
	virtual float GetMinionStrayDistance() const override;

protected:
	virtual void RegisterRTSObject() override;
	virtual void UnRegisterRTSObject() override;
/*************************************************/

/***********IResourceGatherer**********************/
	virtual void AddResource(TSubclassOf<UResource> type, int amount) override;
	virtual bool RemoveResource(const TSubclassOf<UResource> ResourceClass, int amount) override;
	virtual FReplicationResourceMap GetAllHeldResources() const override;
	virtual uint32 GetCurrentWeight() const override;
	virtual uint32 GetMaxWeight() const override;
	virtual uint32 GetResourceMaximum(const TSubclassOf<UResource> ResourceClass) const override;
	virtual uint32 GetResourceMinimum(const TSubclassOf<UResource> ResourceClass) const override;
	virtual FOnResourceValueChangedDelegate& BindResourceValueChangedEvent(const TSubclassOf<UResource> InResourceType) override;
	virtual void AddResourceRegenEvent(FResourceRegenEventConfig InResourceConfig, const TSubclassOf<UResource>& InResourceClass) override;
	virtual bool ClearResourceRegenEvent(const TSubclassOf<UResource>& InResourceClass) override;
	virtual uint32 ClearAllResourceRegenEvents() override;
/*************************************************/

/*********************AActor***********************/
public:
	virtual float TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser) override;

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
/**************************************************/

protected:
	UPROPERTY(EditDefaultsOnly, Category = Behavior)
	UBehaviorTree * RTSBehavior = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Behavior)
	FRTSAIPerceptionConfig AIConfig;

	UPROPERTY(EditDefaultsOnly, Category = Behavior)
	TArray<TSubclassOf<UAISense>> AISenseClasses = TArray<TSubclassOf<UAISense>>();

	UPROPERTY(EditDefaultsOnly, Category = Selection)
	UDecalSelectionComponent * Selection = nullptr;

protected:
	UPROPERTY(EditDefaultsOnly, Category = GamePlay)
	UDeathComponent* DeathComp = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = GamePlay)
	TSubclassOf<UResource> DefaultHealthClass = UHealthResource::StaticClass();

	UPROPERTY(EditDefaultsOnly, Category = GamePlay)
	TMap<TSubclassOf<UDamageType>, TSubclassOf<UResource>> DamageMapping = TMap<TSubclassOf<UDamageType>, TSubclassOf<UResource>>();

	UPROPERTY(EditDefaultsOnly, Category = GamePlay)
	UResourceGathererComponent* ResourceComp = nullptr;

	UPROPERTY(ReplicatedUsing = OnRep_TeamID, EditAnywhere, Category = Gameplay)
	int TeamID = -1;

	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	UTexture* Thumbnail = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	FName MinionName = "Default Minion Name";

	/*Distance the minions will stray from their guard / standpoint*/
	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	float StrayDistance = 4000.0f;

protected:
	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TSubclassOf<UUpgradeData> UpgradeDataClass = UUpgradeData::StaticClass();

protected:
	UPROPERTY(EditDefaultsOnly, Category = Orders)
	const TSubclassOf<URTSMoveOrder> MoveOrderClass = URTSMoveOrder::StaticClass();

protected:
	UPROPERTY(Replicated)
	ACommander * Cmdr = nullptr;

protected:
	bool bAreComponentsReadyforUpgrades = false;

	TArray<TSubclassOf<UUpgrade>> InstalledUpgrades;

protected:
	bool bIsBoxSelectable = false;

	bool bIsLocalEnemy = false;

	FOnUnitDeathDelegate DeathDelegate = FOnUnitDeathDelegate();
};

