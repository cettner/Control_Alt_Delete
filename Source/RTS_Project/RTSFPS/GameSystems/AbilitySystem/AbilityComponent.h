// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Ability.h"
#include "AbilityComponent.generated.h"

constexpr int INVALID_ABILITY_COST = -1;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class RTS_PROJECT_API UAbilityComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAbilityComponent();

public:
	/*Called by Comp Owner on inital attempt to use ability*/
	virtual void StartAbility();
	/*Called by Comp Owner on end of attempt to use ability*/
	virtual void ReleaseAbility();

	/*Called by the ability to notify the component that casting has started*/
	virtual void OnCastStart();
	/*Called by the ability to notify the component that casting has ended*/
	virtual void OnCastEnd();

	/*Start the Abilities Effect*/
	virtual void AbilityEffect();
	/*Notify the ability that it has ended*/
	virtual void EndAbility();

	virtual void ChangeAbility();


public:
	virtual void OnReadyNotify();
	virtual void OnLoopNotify();
	virtual void OnEffectNotify();
	virtual void OnEndNotify();

public:
	virtual TWeakObjectPtr<UAbility> GetCurrentAbility() const;
	virtual bool IsAbilityUsingCrosshair() const;
	virtual bool IsUsingAbility() const;

	/*Creates The Abilty and adds it to the list of available ones this component recieves*/
	virtual TWeakObjectPtr<UAbility> AddAbility(TSubclassOf<UAbility> AbilityClass);

	/*True if the user is currenlty casting or in interim to using the ability, set by the ability after */
	bool IsCasting() const;

	/*True if the user is attempting to use the current ability regardless of mana or other considerations such as timing*/
	bool WantstoCast() const;

	virtual bool IsAbilityValid() const;
	int GetAbilityCost() const;
	bool ConsumeMana(int amount);

public:
	float PlayAbilityMontage(FAbilityAnim PlayAnim);
	virtual bool StopCurrentAnimation();
	AActor * SpawnUninitializedActor(TSubclassOf<AActor> ActorClass, const FTransform &SpawnTransform = FTransform());
	AActor * FinishSpawningActor(AActor * InitializedActor, const FTransform& SpawnTransform);


protected:
	virtual FVector GetControlRotation();
	virtual bool CanUseAbility() const;
	void SetIsCasting(bool CastingState);
	void SetWantsToCast(bool InState);
	void SetCurrentAbility(UAbility * InAbility);

public:
	virtual FTransform GetSurfaceTransform();
	virtual FTransform GetCrosshairTransform(FName Socketname);

protected:
	UPROPERTY()
	UAbility * CurrentAbility = nullptr;
	
	UPROPERTY()
	TArray<UAbility *> AllAbilites;

	FAbilityAnim CurrentMontage;

	bool bWantstoCast = false;
	bool bIsCasting = false;
};
