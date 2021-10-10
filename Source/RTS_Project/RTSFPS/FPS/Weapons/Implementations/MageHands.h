// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../AbilityWeapon.h"
#include "MageHands.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API AMageHands : public AAbilityWeapon
{
	GENERATED_BODY()
	
protected:
	AMageHands();

protected:
	virtual USkeletalMeshComponent* GetWeaponMesh() const override;
	virtual bool InitAbilities() override;

protected:
	/**********************AWeapon Overrides***************************/
	virtual void StartSecondaryFire() override;
	virtual void StopSecondaryFire() override;
	/*****************************************************************/
protected:
	virtual void OnReadyNotify(UAbilityAnimNotify * CallingContext = nullptr) override;
	virtual void OnLoopNotify(UAbilityAnimNotify * CallingContext = nullptr) override;
	virtual void OnEffectNotify(UAbilityAnimNotify * CallingContext = nullptr) override;
	virtual void OnEndNotify(UAbilityAnimNotify * CallingContext = nullptr) override;


protected:
	UAbilityComponent * GetHandAbilityComponent(UAbilityAnimNotify * CallingContext) const;
	
	UFUNCTION(reliable, server, WithValidation)
	void ServerStartUseSecondAbility();

	UFUNCTION(reliable, server, WithValidation)
	void ServerStopUseSecondAbility();


protected:
	/*Container For All Right Hand Assets*/
	UAbilityComponent * RightHandAbilityComp = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UAbility>> RightAbilityClasses;
};
