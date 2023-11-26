// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatCommander.h"
#include "RTS_Project/RTSFPS/GameSystems/AbilitySystem/Interfaces/AbilityUserInterface.h"
#include "RTS_Project/RTSFPS/GameSystems/UpgradeSystem/Interfaces/ExpAccumulatorInterface.h"
#include "RTS_Project/RTSFPS/GameSystems/ResourceSystem/ResourceGathererComponent.h"

#include "AbilityCombatCommander.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API AAbilityCombatCommander : public ACombatCommander, public IAbilityUserInterface, public IExpAccumulatorInterface
{
	GENERATED_BODY()

	
	AAbilityCombatCommander();

	/************************************IAbilityUserInterface****************************/
	public:
		virtual void OnReadyNotify(UAbilityAnimNotify * CallingContext = nullptr) override;
		virtual void OnLoopNotify(UAbilityAnimNotify * CallingContext = nullptr) override;
		virtual void OnEffectNotify(UAbilityAnimNotify * CallingContext = nullptr) override;
		virtual void OnEndNotify(UAbilityAnimNotify * CallingContext = nullptr) override;
		virtual TArray<TWeakObjectPtr<UAbility>> GetAbilitiesByClass(TSubclassOf<UAbility> AbilityClass) const override;
		virtual void AddAbility(TSubclassOf<UAbility> InAbilityClass, AActor* InSource = nullptr) override;
		virtual bool CanCastAbility(const TWeakObjectPtr<UAbility> TracingAbility) const override;
		virtual bool SpendAbilityCost(const TWeakObjectPtr<UAbility> SpendingAbility) override;
	/*************************************************************************************/

	/*********************************Experiance Accumulator***********************************/
		virtual void GrantExp(uint32 inexp) override;
	/******************************************************************************************/
};
