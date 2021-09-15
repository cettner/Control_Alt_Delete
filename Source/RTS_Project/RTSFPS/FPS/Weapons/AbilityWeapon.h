// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "RTS_Project/RTSFPS/GameSystems/AbilitySystem/AbilityComponent.h"
#include "RTS_Project/RTSFPS/GameSystems/AbilitySystem/Interfaces/AbilityUserInterface.h"
#include "AbilityWeapon.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API AAbilityWeapon : public AWeapon, public IAbilityUserInterface
{
	GENERATED_BODY()

	protected:
		AAbilityWeapon();

	public:
	/**********************AWeapon Overrides***************************/
		virtual void StartFire() override;

		virtual void StopFire() override;

		virtual void StartReload(bool bFromReplication = false) override;

		virtual void StopReload() override;
	/*****************************************************************/

	/***********************IAbilityUserInterface*********************/
		virtual bool CanCastAbility() override;
		virtual float PlayAbilityMontage(FAbilityAnim AnimToPlay) override;
		virtual void StopAbilityMontage(FAbilityAnim AnimToStop) override;
		virtual FVector GetAbilitySocketLocation(FName SocketName) override;
		virtual FVector GetAbilityAimVector() const override;
		virtual TArray<AActor *> GetIgnoredTraceActors(TWeakObjectPtr<UAbility> TracingAbility = nullptr) override;

		virtual void OnReadyNotify(UAbilityAnimNotify * CallingContext = nullptr) override;
		virtual void OnLoopNotify(UAbilityAnimNotify * CallingContext = nullptr) override;
		virtual void OnEffectNotify(UAbilityAnimNotify * CallingContext = nullptr) override;
		virtual void OnEndNotify(UAbilityAnimNotify * CallingContext = nullptr) override;
	/*****************************************************************/


	public:
		virtual int GetCurrentMana() const;

	protected:
		virtual void InitAbilities();

	protected:
		virtual void PostInitializeComponents() override;

	protected:

		UFUNCTION(reliable, server, WithValidation)
		void ServerStartUseAbility();

		UFUNCTION(reliable, server, WithValidation)
		void ServerStopUseAbility();

		void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	protected:
		UAbilityComponent * AbilityComp = nullptr;

		UPROPERTY(EditDefaultsOnly)
		TArray<TSubclassOf<UAbility>> AbilityClasses;

		UPROPERTY(replicated)
		int Mana = 0;

		float WeaponRange = 10000.0f;
};
