// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CombatCommander.h"
#include "RTS_Project/RTSFPS/GameSystems/AbilitySystem/Interfaces/AbilityUserInterface.h"
#include "RTS_Project/RTSFPS/GameSystems/UpgradeSystem/Interfaces/ExpAccumulatorInterface.h"
#include "RTS_Project/RTSFPS/GameSystems/ResourceSystem/ResourceGathererComponent.h"
#include "RTS_Project/RTSFPS/GameSystems/ResourceSystem/Interfaces/ResourceVendorInterface.h"
#include "RTS_Project/RTSFPS/GameSystems/ResourceSystem/ResourceVendorComponent.h"

#include "AbilityCombatCommander.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API AAbilityCombatCommander : public ACombatCommander, public IAbilityUserInterface, public IResourceGatherer, public IResourceVendorInterface, public IExpAccumulatorInterface
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


	/*********************************Resource Gatherer***********************************/
	public:
		virtual void AddResource(TSubclassOf<UResource> ResourceClass, int amount) override;
		virtual bool RemoveResource(TSubclassOf<UResource> ResourceClass, int amount) override;
		virtual FReplicationResourceMap GetAllHeldResources() const override;
		virtual uint32 GetCurrentWeight() const override;
		virtual uint32 GetMaxWeight() const override;
		virtual uint32 GetResourceMaximum(const TSubclassOf<UResource> ResourceClass) const override;
		virtual uint32 GetResourceMinimum(const TSubclassOf<UResource> ResourceClass) const override;
	/*************************************************************************************/
	
	/*********************************Resource Vendor***********************************/
		virtual const TMap<TSubclassOf<UObject>, FReplicationResourceMap> GetAllDefaultUnitPrices() const override;
		virtual bool GetUnitPriceForSource(const TSubclassOf<UObject> PurchaseClass, const IResourceGatherer* Purchaser, FReplicationResourceMap& OutPrices, const AController* InstigatingController = nullptr) const override;
	/***********************************************************************************/

	/*********************************Experiance Accumulator***********************************/
		virtual void GrantExp(uint32 inexp) override;
	/******************************************************************************************/

	protected:
		virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;


	/****************************************Config Data***************************************/
		UPROPERTY(EditDefaultsOnly, Category = Gameplay)
		UResourceVendorComponent* ResourceVendorComp = nullptr;

	/************************************Runtime Data*****************************************/


};
