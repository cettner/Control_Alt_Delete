// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RTSSelectionComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "HealthComponent.h"
#include "RTSStructure.h"
#include "RTSMinion.generated.h"


/*Forward Declarations*/
class ACommander;

UCLASS(Blueprintable)
class ARTSMinion : public ACharacter
{
	GENERATED_BODY()

public:
	ARTSMinion();

	virtual void BeginPlay() override;
	
	virtual float TakeDamage(float Damage, FDamageEvent const & DamageEvent, AController * EventInstigator, AActor * DamageCauser) override;

	UFUNCTION(BlueprintCallable, Category = UI)
	UTexture* GetThumbnail();

	UPROPERTY(EditDefaultsOnly,Category = UI)
	UTexture* Thumbnail;

	virtual bool CanInteract(AActor * Interactable);

	virtual bool CanAttack(AActor * AttackMe);

	virtual bool CanDoDamage(AActor * AttackMe);

	virtual void StartAttack(AActor * AttackMe);

	virtual bool IsAlive();

	virtual bool IsEnemy(AActor *  InMinion);

	virtual AActor* GetTarget();

	virtual void SetTarget(AActor * NewTarget);

	virtual void ClearTarget();

	void SetSelected();

	void SetDeselected();

	void SetUnselectable();

	void SetSelectable();

	virtual void ReleaseAssets();

	virtual bool HasAssets();

	virtual void RtsMove(FVector Local);
	
	virtual void RtsMoveToActor(AActor * move_to_me);

	virtual ACommander * GetCommander();

	virtual void ClearCommander();

	virtual void SetCommander(ACommander * Commander);

	//TODO: Make this a Server Call
	virtual void SetTeam(int team_id);

	virtual int GetTeam();

	virtual UBehaviorTree* GetBehavior();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UHealthComponent * Health;

	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	int team_index = -1;

	UPROPERTY(EditAnywhere, Category = Behavior)
	UBehaviorTree * RTSBehavior;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Selection)
	URTSSelectionComponent * Selection;

	UPROPERTY(Replicated)
	ACommander * Cmdr;

};

