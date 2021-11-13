// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "BehaviorTree/BehaviorTree.h"

#include "RTS_Project/RTSFPS/GameSystems/UpgradeSystem/Interfaces/UpgradableInterface.h"
#include "RTS_Project/RTSFPS/BaseClasses/Interfaces/RTSObjectInterface.h"
#include "RTS_Project/RTSFPS/GameSystems/HealthSystem/HealthComponent.h"
#include "RTS_Project/RTSFPS/Shared/Components/DecalSelectionComponent.h"
#include "RTS_Project/RTSFPS/RTS/Minions/AI/RTSAIController.h"
#include "RTSMinion.generated.h"


/*Forward Declarations*/
class ACommander;

UCLASS(Blueprintable)
class ARTSMinion : public ACharacter, public IRTSObjectInterface, public IUpgradableInterface
{
	GENERATED_BODY()

public:
	ARTSMinion();

	virtual bool CanInteract(AActor * Interactable);

	virtual bool CanAttack(AActor * AttackMe);

	virtual bool CanDoDamage(AActor * AttackMe);

	virtual void StartAttack(AActor * AttackMe);

	virtual bool IsAlive() const;

	UFUNCTION()
	virtual void OnDeath();

	virtual bool IsEnemy(AActor *  InMinion);

	virtual AActor* GetTarget();

	UFUNCTION(BlueprintCallable)
	virtual void SetTarget(AActor * NewTarget);

	virtual void ClearTarget();

	virtual void ReleaseAssets();

	virtual bool HasAssets();

	virtual void RtsMove(FVector Local);

	virtual void RtsMoveToActor(AActor * move_to_me);

	virtual ACommander * GetCommander();

	virtual void ClearCommander();

	virtual void SetCommander(ACommander * Commander);

	UFUNCTION()
	virtual void OnRep_TeamID();

	virtual UBehaviorTree* GetBehavior();

	virtual FRTSAIPerceptionConfig GetAIConfig() const;

/**************IRTSObjectInterface****************/
	virtual void SetSelected()  override;

	virtual void SetDeselected() override;

	virtual void SetTeam(int team_id) override;

	virtual int GetTeam() const override;

	virtual void SetTeamColors(FLinearColor TeamColor) override; 
/*************************************************/

/**************IUpgradableInterface****************/
protected:
	virtual void PostInstallUpgrades() override;
	virtual bool CanReceiveUpgrades() const override;
	virtual bool AddUpgrade(TSubclassOf<UUpgrade> UpgradeToAdd) override;
/**************************************************/

/*********************AActor***********************/
public:
	virtual float TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser) override;

protected:
	virtual void PostInitializeComponents() override;
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
/**************************************************/

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UHealthComponent * Health;

	UPROPERTY(ReplicatedUsing = OnRep_TeamID, EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	int team_index = -1;

	UPROPERTY(EditAnywhere, Category = Behavior)
	UBehaviorTree * RTSBehavior;

	UPROPERTY(EditDefaultsOnly, Category = Behavior)
	FRTSAIPerceptionConfig AIConfig;

	UPROPERTY(EditDefaultsOnly, Category = Selection)
	UDecalSelectionComponent * Selection = nullptr;

	UPROPERTY(Replicated)
	ACommander * Cmdr;

protected:
	bool bAreComponentsReadyforUpgrades = false;
	TArray<TSubclassOf<UUpgrade>> AppliedUpgrades;
};

