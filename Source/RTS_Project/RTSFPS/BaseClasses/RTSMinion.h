// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RTS_Project/RTSFPS/GameSystems/HealthSystem/HealthComponent.h"
#include "RTS_Project/RTSFPS/RTS/Camera/RTSSelectionComponent.h"
#include "RTS_Project/RTSFPS/RTS/Minions/AI/RTSAIController.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "RTSMinion.generated.h"


/*Forward Declarations*/
class ACommander;

UCLASS(Blueprintable)
class ARTSMinion : public ACharacter
{
	GENERATED_BODY()

public:
	ARTSMinion();
	
	virtual float TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser) override;

	UFUNCTION(BlueprintCallable, Category = UI)
	UTexture* GetThumbnail();

	virtual bool CanInteract(AActor * Interactable);

	virtual bool CanAttack(AActor * AttackMe);

	virtual bool CanDoDamage(AActor * AttackMe);

	virtual void StartAttack(AActor * AttackMe);

	virtual bool IsAlive();

	UFUNCTION()
	virtual void OnDeath();

	virtual bool IsEnemy(AActor *  InMinion);

	virtual AActor* GetTarget();

	virtual void SetTarget(AActor * NewTarget);

	virtual void ClearTarget();

	virtual void SetSelected();

	virtual void SetDeselected();

	virtual void SetTeamColors();

	virtual void ReleaseAssets();

	virtual bool HasAssets();

	virtual void RtsMove(FVector Local);
	
	virtual void RtsMoveToActor(AActor * move_to_me);

	virtual ACommander * GetCommander();

	virtual void ClearCommander();

	virtual void SetCommander(ACommander * Commander);

	//TODO: Make this a Server Call
	virtual void SetTeam(int team_id);

	virtual int GetTeam() const;

	UFUNCTION()
	virtual void OnRep_TeamID();

	virtual UBehaviorTree* GetBehavior();

	virtual FRTSAIPerceptionConfig GetAIConfig() const;

protected:
	virtual void PostInitializeComponents() override;

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UHealthComponent * Health;

	UPROPERTY(ReplicatedUsing = OnRep_TeamID, EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	int team_index = -1;

	UPROPERTY(EditAnywhere, Category = Behavior)
	UBehaviorTree * RTSBehavior;

	UPROPERTY(EditDefaultsOnly, Category = Perception)
	FRTSAIPerceptionConfig AIConfig;
	
	UPROPERTY(Editdefaultsonly, Category = Selection)
	URTSSelectionComponent * Selection;

	UPROPERTY(Replicated)
	ACommander * Cmdr;

	UPROPERTY(EditDefaultsOnly, Category = UI)
	UTexture* Thumbnail;



};

