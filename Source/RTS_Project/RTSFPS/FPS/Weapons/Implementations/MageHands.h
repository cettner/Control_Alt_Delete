// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../AbilityWeapon.h"
#include "RTS_Project/RTSFPS/GameSystems/ResourceSystem/Interfaces/ResourceGatherer.h"
#include "MageHands.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API AMageHands : public AAbilityWeapon, public IResourceGatherer
{
	GENERATED_BODY()
	
protected:
	AMageHands();
	virtual void PostInitializeComponents() override;

protected:
	/******************AAilityWeapon Overrides************************/
	virtual bool InitAbilities(IAbilityUserInterface * ) override;
	virtual void OnResourceSourceChanged(const TSubclassOf<UResource> InClass, const uint32 InOldValue, const uint32 InNewValue, TScriptInterface<IResourceGatherer> InSource) override;
	/*****************************************************************/

protected:
	/**********************AWeapon Overrides***************************/
	virtual void StartSecondaryFire() override;
	virtual void StopSecondaryFire() override;
	virtual USkeletalMeshComponent* GetWeaponMesh() const override;
	/*****************************************************************/

	/*********************IAbilityInterface*************************/
protected:
	virtual void OnReadyNotify(UAbilityAnimNotify * CallingContext = nullptr) override;
	virtual void OnLoopNotify(UAbilityAnimNotify * CallingContext = nullptr) override;
	virtual void OnEffectNotify(UAbilityAnimNotify * CallingContext = nullptr) override;
	virtual void OnEndNotify(UAbilityAnimNotify * CallingContext = nullptr) override;
	virtual TArray<TWeakObjectPtr<UAbility>> GetAbilitiesByClass(const TSubclassOf<UAbility>& InAbilityClass) const override;
	virtual TWeakObjectPtr<UAbility> GetFirstAbilityByClass(const TSubclassOf<UAbility>& InAbilityClass) const;
	virtual void EnableAbility(const TSubclassOf<UAbility>& AbilityClass) override;
	virtual bool DisableAbility(const TSubclassOf<UAbility>& AbilityClass) override;
	virtual TSet<TSubclassOf<UAbility>> GetSupportedAbilities() const override;
	/***************************************************************/

protected:
	/**********************Resource Gatherer Overrides***************************/
	virtual void AddResource(TSubclassOf<UResource> ResourceClass, int amount) override;
	virtual bool RemoveResource(TSubclassOf<UResource> ResourceClass, int amount) override;
	virtual uint32 GetCurrentWeight() const override;
	virtual uint32 GetMaxWeight() const override;
	virtual FReplicationResourceMap GetAllHeldResources() const override;
	/****************************************************************************/

protected:
	UAbilityComponent * GetHandAbilityComponent(UAbilityAnimNotify * CallingContext) const;
	
	UFUNCTION(reliable, server, WithValidation)
	void ServerStartUseSecondAbility(int InAbilityIndex);

	UFUNCTION(reliable, server, WithValidation)
	void ServerStopUseSecondAbility();

	UFUNCTION()
	virtual void OnRightHandAbilityEnableStateChanged(UAbility * InChangedAbility);

protected:
	UPROPERTY(EditDefaultsOnly)
	FName OffHandAbilityTag = "OffHand";

	UPROPERTY(EditDefaultsOnly)
	FName MainHandAbilityTag = "MainHand";


protected:
	/*Container For All Right Hand Assets*/
	UPROPERTY(EditDefaultsOnly, Category = Abilities)
	UAbilityComponent* RightHandAbilityComp = nullptr;

	int RightHandAbilityIndex = -1;
};
