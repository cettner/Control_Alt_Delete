// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Ability.h"
#include "Interfaces/AbilityUserInterface.h"
#include "AbilityComponent.generated.h"

constexpr int NO_ABILITY_INDEX = -1;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityEnableStateChangedDelegate, TWeakObjectPtr<UAbility>, InChangedAbility);

USTRUCT()
struct FAbilityReplicationBool
{
	GENERATED_USTRUCT_BODY()

public:
	void Set(bool InSuccess, int InAbilityIndex)
	{
		Success = InSuccess;
		AbilityIndex = InAbilityIndex;
		ReplicationCounter++;
	}

	bool WasSuccessful() const
	{
		return(Success);
	}

	int Index() const
	{
		return AbilityIndex;
	}


private:
	UPROPERTY()
	bool Success = false;

	UPROPERTY()
	uint8 ReplicationCounter = 0x00U;

	UPROPERTY()
	int AbilityIndex = -1;
};

UCLASS( ClassGroup=(Custom))
class RTS_PROJECT_API UAbilityComponent : public UActorComponent
{
	GENERATED_BODY()
	friend UAbility;


public:	
	// Sets default values for this component's properties
	UAbilityComponent();
	void InitAbilities(IAbilityUserInterface * InAbilitiyUser, TArray<TSubclassOf<UAbility>> InAllAbilityClasses);

public:
	/*Pass through function for Abilities to dermine if they should Execute Authoritative Commands based on the comps owner*/
	bool HasAuthority() const;

public:
	/*Called by Comp Owner on inital attempt to use Ability*/
	virtual void StartAbility(int InAbilityIndex);
	/*Called by Comp Owner on end of attempt to use Ability*/
	virtual void ReleaseAbility();
	/*Called By Comp Owner to Immediatly Halt the Ability*/
	virtual void InterruptAbility();
	
	/****************Server Trigger***********/
	/*Called by Ability to notify that the ability was successfully started*/
	void SetIsCastSuccessful(bool ReleaseState);
	/*****************************************/

public:
	/*Start the Abilities Effect*/
	virtual void AbilityEffect();
	/*Notify the ability that it has ended*/
	virtual void EndAbility();

public:
	virtual void OnReadyNotify();
	virtual void OnLoopNotify();
	virtual void OnEffectNotify();
	virtual void OnEndNotify();

public:
	virtual bool IsAbilityEnabled(const int InIndex) const;
	/****************Server Trigger***********/
	virtual bool SetAbilityEnabledState(int InAbilityIndex, bool InEnabledState);
	virtual bool SetAbilityEnabledState(TSubclassOf<UAbility> InEnabledClass, bool InEnabledState);
	/*****************************************/

public:
	virtual UAbility * GetCurrentAbility() const;
	virtual UAbility * GetAbilityByIndex(int InIndex) const;
	virtual int GetAbilityIndexByClass(TSubclassOf<UAbility> InEnabledClass) const;
	virtual int GetCurrentAbilityIndex() const;
	virtual int GetNextAvailableIndex(const int InCurrentIndex = NO_ABILITY_INDEX, const bool bOnlyEnabledAbilities = true) const;

	virtual bool IsAbilityUsingCrosshair() const;
	virtual bool IsUsingAbility() const;



	/*True if the user is currenlty casting or in interim to using the ability, set by the ability after */
	bool IsCasting() const;

	/*True if the user is attempting to use the current ability regardless of mana or other considerations such as timing*/
	bool WantstoCast() const;

	/*True if the cast is complete and is awaiting release / execution*/
	bool IsCastReady() const;

	virtual bool IsAbilityValid() const;
	virtual bool IsAbilityValid(int AbilityIndex) const;

	/*Returns the User Associated witht the Component*/
	virtual IAbilityUserInterface * GetAbilityUser() const;

	virtual TArray<TWeakObjectPtr<UAbility>> GetAbilitiesByClass(TSubclassOf<UAbility> AbilityClass) const;

public:
	float PlayAbilityMontage(FAbilityAnim PlayAnim);
	virtual bool StopCurrentAnimation();
	AActor * SpawnUninitializedActor(TSubclassOf<AActor> ActorClass, const FTransform &SpawnTransform = FTransform());
	FORCEINLINE const FAbilityAnim& GetCurrentMontage() const { return CurrentMontage; }
	template < class T >
	T * SpawnUninitializedActor(TSubclassOf<AActor> Class, const FTransform &SpawnTransform)
	{
		T* retval = Cast<T>(SpawnUninitializedActor(Class, SpawnTransform));
		return(retval);
	}

	AActor * FinishSpawningActor(AActor * InitializedActor, const FTransform& SpawnTransform);

public:
	virtual FTransform GetSurfaceTransform();
	virtual FTransform GetCrosshairTransform(FName Socketname);

protected:
	/*Called by the ability to notify the component that casting has started*/
	virtual void OnCastStart();
	/*Called by the ability to notify the component that casting has ended*/
	virtual void OnCastEnd();

	virtual void OnAbilityEnableStateChanged(UAbility* InAbility);

protected:
	virtual FVector GetControlRotation();
	virtual int GetNextEnabledIndex(int StartIndex = 0) const;
	virtual bool CanUseAbility(int AbilityIndex) const;
	
	/*ServerReplication Triggers*/
	void SetIsCasting(bool CastingState);
	void SetIsCastReady(bool ReadyState);
	/*****************************/

	void SetWantsToCast(bool InState);
	void SetCurrentAbility(int InAbilityIndex);



public:
	FOnAbilityEnableStateChangedDelegate AbilityChangeDelegate = FOnAbilityEnableStateChangedDelegate();

protected:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;
	
	UFUNCTION()
	void OnRep_bIsCasting();

	UFUNCTION()
	void OnRep_bIsCastReleased();

	UFUNCTION()
	void OnRep_AllAbilities();

protected:
	IAbilityUserInterface* AbilityUser = nullptr;

	int CurrentAbilityIndex = -1;

	FAbilityAnim CurrentMontage;

	bool bWantstoCast = false;

	bool bAbilitiesInitialized = false;

protected:
	/*Have to mark this as UPROPERTY or the UObjects are considered Weak Pointers without reference and are "randomly" garbage collected*/
	UPROPERTY(ReplicatedUsing = OnRep_AllAbilities)
	TArray<UAbility*> AllAbilites = TArray<UAbility*>();

private:
	bool bIsCastReady = false;

	UPROPERTY(ReplicatedUsing = OnRep_bIsCasting)
	FAbilityReplicationBool bIsCasting = FAbilityReplicationBool();

	UPROPERTY(ReplicatedUsing = OnRep_bIsCastReleased)
	FAbilityReplicationBool bReleaseSuccess = FAbilityReplicationBool();

};
