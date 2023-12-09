// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatCommander.h"
#include "RTS_Project/RTSFPS/GameSystems/AbilitySystem/Interfaces/AbilityUserInterface.h"
#include "RTS_Project/RTSFPS/GameSystems/UpgradeSystem/Interfaces/ExpAccumulatorInterface.h"
#include "RTS_Project/RTSFPS/GameSystems/ResourceSystem/ResourceGathererComponent.h"
#include "RTS_Project/RTSFPS/FPS/Upgrades/Abilities/AbilityUserUpgrade.h"

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
		virtual void OnTickNotify(float InFrameDeltaTime, const FAnimNotifyEventReference& InEventReference) override;
		virtual TArray<UAbility*> GetAbilitiesByClass(const TSubclassOf<UAbility>& InAbilityClass) const override;
		virtual UAbility* GetFirstAbilityByClass(const TSubclassOf<UAbility>& InAbilityClass) const override;
		virtual void EnableAbility(const TSubclassOf<UAbility>& AbilityClass) override;
		virtual bool DisableAbility(const TSubclassOf<UAbility>& AbilityClass) override;
		virtual bool IsAbilityEnabled(const TSubclassOf<UAbility>& AbilityClass) const override;
		virtual TSet<TSubclassOf<UAbility>> GetSupportedAbilities() const override;
		virtual bool SupportsAbility(const TSubclassOf<UAbility>& Inabilityclass) const;
		virtual bool CanCastAbility(const UAbility * TracingAbility) const override;
		virtual bool SpendAbilityCost(const UAbility * SpendingAbility) override;
	/*************************************************************************************/

	/*********************************Experiance Accumulator***********************************/
		virtual void GrantExp(uint32 InExp) override;
	/******************************************************************************************/

	/***********************************ARTSMinion*********************************************/
		virtual UObject* GetUpgradeSubObject(const TSubclassOf<UUpgrade>& InUpgradeclass) const override;
	/******************************************************************************************/

};
