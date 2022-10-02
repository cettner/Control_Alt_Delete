// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatCommander.h"
#include "RTS_Project/RTSFPS/GameSystems/AbilitySystem/Interfaces/AbilityUserInterface.h"
#include "RTS_Project/RTSFPS/GameSystems/UpgradeSystem/Interfaces/ExpAccumulatorInterface.h"
#include "RTS_Project/RTSFPS/GameSystems/ResourceSystem/Interfaces/ResourceGatherer.h"
#include "AbilityCombatCommander.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API AAbilityCombatCommander : public ACombatCommander, public IAbilityUserInterface, public IResourceGatherer, public IExpAccumulatorInterface
{
	GENERATED_BODY()

	protected:
		/*Used to Optimize Retrieval of Resources*/
		void CalculateCurrentWeight();


	/************************************IAbilityUserInterface****************************/
	public:
		virtual void OnReadyNotify(UAbilityAnimNotify * CallingContext = nullptr) override;
		virtual void OnLoopNotify(UAbilityAnimNotify * CallingContext = nullptr) override;
		virtual void OnEffectNotify(UAbilityAnimNotify * CallingContext = nullptr) override;
		virtual void OnEndNotify(UAbilityAnimNotify * CallingContext = nullptr) override;
		virtual TArray<TWeakObjectPtr<UAbility>> GetAbilitiesByClass(TSubclassOf<UAbility> AbilityClass) const override;
		virtual void AddAbility(TSubclassOf<UAbility> InAbilityClass, AActor* InSource = nullptr) override;
	/*************************************************************************************/



	/*********************************Resource Gatherer***********************************/
	public:
		virtual void AddResource(TSubclassOf<AResource> ResourceClass, int amount);
		virtual bool RemoveResource(TSubclassOf<AResource> ResourceClass, int amount);

	/*Returns the count of the specified class that the gatherer can carry until the cap is achieved.*/
		virtual uint32 CanCarryMore(TSubclassOf<AResource> ResourceClass) const;
		//virtual bool DropsResourceOnDeath() const;
		virtual FReplicationResourceMap GetAllHeldResources() const;
		//virtual uint32 GetHeldResource(TSubclassOf<AResource> ResourceClass) const;
		virtual uint32 GetCurrentWeight() const;
		virtual uint32 GetMaxWeight() const;
	/*************************************************************************************/
	

	/*********************************Experiance Accumulator***********************************/
		virtual void GrantExp(uint32 inexp) override;
	/******************************************************************************************/

	protected:
		virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;


	/****************************************Config Data***************************************/
	protected:
		UPROPERTY(EditDefaultsOnly, Replicated)
		uint32 MaxWeight = 30U;

	/************************************Runtime Data*****************************************/

		FReplicationResourceMap HeldResources = FReplicationResourceMap();
		
		UPROPERTY(Replicated)
		uint32 CurrentWeight = 0U;

};
