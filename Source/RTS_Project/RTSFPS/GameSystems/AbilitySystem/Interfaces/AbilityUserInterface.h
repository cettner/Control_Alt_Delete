// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "../Animation/AbilityAnimNotify.h"
#include"../Ability.h"
#include "AbilityUserInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UAbilityUserInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RTS_PROJECT_API IAbilityUserInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
	public:
		virtual bool CanCastAbility(const TWeakObjectPtr<UAbility> TracingAbility) const;
		virtual bool SpendAbilityCost(const TWeakObjectPtr<UAbility> SpendingAbility);
		virtual float PlayAbilityMontage(const FAbilityAnim& AnimToPlay);
		virtual void  StopAbilityMontage(const FAbilityAnim& AnimToStop);
		virtual FVector GetAbilitySocketLocation(FName SocketName) const;
		virtual FVector GetAbilityAimVector() const;
		virtual TArray<AActor *> GetIgnoredTraceActors(TWeakObjectPtr<UAbility> TracingAbility = nullptr);

		virtual TWeakObjectPtr<UAbility> GetFirstAbilityByClass(const TSubclassOf<UAbility>& InAbilityClass) const;
		virtual TArray<TWeakObjectPtr<UAbility>> GetAbilitiesByClass(const TSubclassOf<UAbility>& InAbilityClass) const;
		virtual TArray<TWeakObjectPtr<UAbility>> GetAbilitiesByTag(FName InTag) const;
		
		virtual void EnableAbility(const TSubclassOf<UAbility>& AbilityClass);
		virtual bool DisableAbility(const TSubclassOf<UAbility>& AbilityClass);
		virtual bool IsAbilityEnabled(const TSubclassOf<UAbility>& AbilityClass) const;
		
		virtual TSet<TSubclassOf<UAbility>> GetSupportedAbilities() const;
		virtual bool SupportsAbility(const TSubclassOf<UAbility>& Inabilityclass) const;

		virtual void OnReadyNotify(UAbilityAnimNotify * CallingContext = nullptr);
		virtual void OnLoopNotify(UAbilityAnimNotify * CallingContext = nullptr);
		virtual void OnEffectNotify(UAbilityAnimNotify * CallingContext = nullptr);
		virtual void OnEndNotify(UAbilityAnimNotify * CallingContext = nullptr);

		virtual USceneComponent * GetParticleAttatchmentComponent(TWeakObjectPtr<UAbility> SpawningAbility = nullptr);
};
