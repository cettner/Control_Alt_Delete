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

		virtual void OnEnterInventory(ACombatCommander* NewOwner) override;

		virtual void WeaponPrimarySetting(int ModeToggle) override;
	/*****************************************************************/

	/***********************IAbilityUserInterface*********************/
		virtual bool CanCastAbility() const override;
		virtual float PlayAbilityMontage(FAbilityAnim AnimToPlay) override;
		virtual void StopAbilityMontage(FAbilityAnim AnimToStop) override;
		virtual FVector GetAbilitySocketLocation(FName SocketName) const override;
		virtual FVector GetAbilityAimVector() const override;
		virtual TArray<AActor *> GetIgnoredTraceActors(TWeakObjectPtr<UAbility> TracingAbility = nullptr) override;
		virtual TArray<TWeakObjectPtr<UAbility>> GetAbilitiesByClass(TSubclassOf<UAbility> AbilityClass) const override;

		virtual void OnReadyNotify(UAbilityAnimNotify * CallingContext = nullptr) override;
		virtual void OnLoopNotify(UAbilityAnimNotify * CallingContext = nullptr) override;
		virtual void OnEffectNotify(UAbilityAnimNotify * CallingContext = nullptr) override;
		virtual void OnEndNotify(UAbilityAnimNotify * CallingContext = nullptr) override;

		virtual USceneComponent * GetParticleAttatchmentComponent(TWeakObjectPtr<UAbility> SpawningAbility = nullptr) override;
	/*****************************************************************/


	public:
		virtual int GetCurrentMana() const;

	protected:
		virtual bool InitAbilities();

	protected:
		virtual void EndPlay(EEndPlayReason::Type InReason) override;

	protected:

		UFUNCTION(reliable, server, WithValidation)
		void ServerStartUseAbility(int AbilityIndextoStart);

		UFUNCTION(reliable, server, WithValidation)
		void ServerStopUseAbility();

		void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	protected:
		UAbilityComponent * AbilityComp = nullptr;

		bool bAreAbilitiesInitialized = false;

		UPROPERTY(EditDefaultsOnly)
		TArray<TSubclassOf<UAbility>> AbilityClasses;

		UPROPERTY(replicated)
		int Mana = 0;

		/*Ability is indexed locally*/
		int AbilityIndex = -1;
};
