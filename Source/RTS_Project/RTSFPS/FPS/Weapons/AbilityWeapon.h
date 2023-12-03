// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "RTS_Project/RTSFPS/GameSystems/AbilitySystem/AbilityComponent.h"
#include "RTS_Project/RTSFPS/GameSystems/AbilitySystem/Interfaces/AbilityUserInterface.h"
#include "../../GameSystems/ResourceSystem/Interfaces/ResourceGatherer.h"

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
		virtual bool CanCastAbility(const UAbility *) const override;
		virtual bool SpendAbilityCost(const UAbility * SpendingAbility) override;
		virtual float PlayAbilityMontage(const FAbilityAnim& AnimToPlay) override;
		virtual void StopAbilityMontage(const FAbilityAnim& AnimToPlay) override;
		virtual FVector GetAbilitySocketLocation(FName SocketName) const override;
		virtual FVector GetAbilityAimVector() const override;
		virtual TArray<AActor *> GetIgnoredTraceActors(TWeakObjectPtr<UAbility> TracingAbility = nullptr) override;
		virtual TArray<UAbility *> GetAbilitiesByClass(const TSubclassOf<UAbility>& AbilityClass) const override;
		virtual void EnableAbility(const TSubclassOf<UAbility>& AbilityClass) override;
		virtual bool DisableAbility(const TSubclassOf<UAbility>& AbilityClass) override;
		virtual TSet<TSubclassOf<UAbility>> GetSupportedAbilities() const override;
		virtual UAbility * GetFirstAbilityByClass(const TSubclassOf<UAbility>& InAbilityClass) const override;


		virtual void OnReadyNotify(UAbilityAnimNotify * CallingContext = nullptr) override;
		virtual void OnLoopNotify(UAbilityAnimNotify * CallingContext = nullptr) override;
		virtual void OnEffectNotify(UAbilityAnimNotify * CallingContext = nullptr) override;
		virtual void OnEndNotify(UAbilityAnimNotify * CallingContext = nullptr) override;

		virtual USceneComponent * GetParticleAttatchmentComponent(TWeakObjectPtr<UAbility> SpawningAbility = nullptr) override;
	/*****************************************************************/

	protected:
		virtual bool InitAbilities(IAbilityUserInterface * InUser);
		virtual void InitResourceBindings(IResourceGatherer * InResourceSource);

		UFUNCTION()
		virtual void OnAbilityEnableStateChanged(UAbility * InChangedAbility);

		virtual void OnResourceSourceChanged(const TSubclassOf<UResource> InClass, const uint32 InOldValue, const uint32 InNewValue, TScriptInterface<IResourceGatherer> InSource);

	protected:
		virtual void PostInitializeComponents() override;
		virtual void EndPlay(EEndPlayReason::Type InReason) override;
		virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	protected:
		UFUNCTION(reliable, server, WithValidation)
		void ServerStartUseAbility(int AbilityIndextoStart);

		UFUNCTION(reliable, server, WithValidation)
		void ServerStopUseAbility();

	protected:
		/*Config*/
		UPROPERTY(EditDefaultsOnly, Category = Abilities)
		UAbilityComponent * AbilityComp = nullptr;

	protected:
		/*Runtime*/
		bool bAreAbilitiesInitialized = false;

		/*Ability is indexed locally*/
		int AbilityIndex = -1;
};
