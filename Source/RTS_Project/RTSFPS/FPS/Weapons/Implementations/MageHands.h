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
	virtual bool InitAbilities(IAbilityUserInterface * ) override;

protected:
	/**********************AWeapon Overrides***************************/
	virtual void StartSecondaryFire() override;
	virtual void StopSecondaryFire() override;
	virtual USkeletalMeshComponent* GetWeaponMesh() const override;
	/*****************************************************************/
protected:
	virtual void OnReadyNotify(UAbilityAnimNotify * CallingContext = nullptr) override;
	virtual void OnLoopNotify(UAbilityAnimNotify * CallingContext = nullptr) override;
	virtual void OnEffectNotify(UAbilityAnimNotify * CallingContext = nullptr) override;
	virtual void OnEndNotify(UAbilityAnimNotify * CallingContext = nullptr) override;
	virtual TArray<TWeakObjectPtr<UAbility>> GetAbilitiesByClass(TSubclassOf<UAbility> AbilityClass) const override;
	virtual void AddAbility(TSubclassOf<UAbility> InAbilityClass, AActor* InSource = nullptr) override;

protected:
	/**********************Resource Gatherer Overrides***************************/
	virtual void AddResource(TSubclassOf<AResource> ResourceClass, int amount) override;
	virtual uint32 GetCurrentWeight() const override;
	virtual uint32 GetMaxWeight() const override;
	/****************************************************************************/

protected:
	UAbilityComponent * GetHandAbilityComponent(UAbilityAnimNotify * CallingContext) const;
	
	UFUNCTION(reliable, server, WithValidation)
	void ServerStartUseSecondAbility(int InAbilityIndex);

	UFUNCTION(reliable, server, WithValidation)
	void ServerStopUseSecondAbility();

	UFUNCTION()
	virtual void OnRightHandAbilityEnableStateChanged(TArray<int> InChangedAbilityIndicies);

protected:
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UAbility>> RightAbilityClasses;

	UPROPERTY(EditDefaultsOnly)
	FName OffHandAbilityTag = "OffHand";

	UPROPERTY(EditDefaultsOnly)
	FName MainHandAbilityTag = "MainHand";


protected:
	/*Container For All Right Hand Assets*/
	UAbilityComponent* RightHandAbilityComp = nullptr;

	int RightHandAbilityIndex = -1;
};
