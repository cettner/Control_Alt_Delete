// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityCombatCommander.h"

#include "Net/UnrealNetwork.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/PlayerController.h"

void AAbilityCombatCommander::OnReadyNotify(UAbilityAnimNotify * CallingContext)
{
	IAbilityUserInterface * abilityweapon = Cast<IAbilityUserInterface>(CurrentWeapon);

	if (abilityweapon != nullptr)
	{
		abilityweapon->OnReadyNotify(CallingContext);
	}
}

void AAbilityCombatCommander::OnLoopNotify(UAbilityAnimNotify * CallingContext)
{
	IAbilityUserInterface * abilityweapon = Cast<IAbilityUserInterface>(CurrentWeapon);

	if (abilityweapon != nullptr)
	{
		abilityweapon->OnLoopNotify(CallingContext);
	}
}

void AAbilityCombatCommander::OnEffectNotify(UAbilityAnimNotify * CallingContext)
{
	IAbilityUserInterface * abilityweapon = Cast<IAbilityUserInterface>(CurrentWeapon);

	if (abilityweapon != nullptr)
	{
		abilityweapon->OnEffectNotify(CallingContext);
	}
}

void AAbilityCombatCommander::OnEndNotify(UAbilityAnimNotify * CallingContext)
{
	IAbilityUserInterface * abilityweapon = Cast<IAbilityUserInterface>(CurrentWeapon);

	if (abilityweapon != nullptr)
	{
		abilityweapon->OnEndNotify(CallingContext);
	}
}

TArray<TWeakObjectPtr<UAbility>> AAbilityCombatCommander::GetAbilitiesByClass(TSubclassOf<UAbility> AbilityClass) const
{
	IAbilityUserInterface * abilityweapon = Cast<IAbilityUserInterface>(CurrentWeapon);
	TArray<TWeakObjectPtr<UAbility>> retval = TArray<TWeakObjectPtr<UAbility>>();
	if (abilityweapon != nullptr)
	{
		retval = abilityweapon->GetAbilitiesByClass(AbilityClass);
	}
	return retval;
}

void AAbilityCombatCommander::AddAbility(TSubclassOf<UAbility> InAbilityClass, AActor* InSource)
{
	IAbilityUserInterface* abilityweapon = Cast<IAbilityUserInterface>(CurrentWeapon);

	abilityweapon->AddAbility(InAbilityClass, InSource);
}

void AAbilityCombatCommander::AddResource(TSubclassOf<AResource> InResourceClass, int InAmount)
{
	const bool retval = HeldResources.Increment(InResourceClass, InAmount);
	if (retval == true)
	{
		const AResource* resourcecdo = InResourceClass.GetDefaultObject();
		const int resourceweight = resourcecdo->GetResourceWeight();
		CurrentWeight += (resourceweight * InAmount);
	}

}

bool AAbilityCombatCommander::RemoveResource(TSubclassOf<AResource> InResourceClass, int InAmount)
{
	const bool retval = HeldResources.Decrement(InResourceClass, InAmount);
	if (retval == true)
	{
		const AResource* resourcecdo = InResourceClass.GetDefaultObject();
		const int resourceweight = resourcecdo->GetResourceWeight();
		CurrentWeight -= (resourceweight * InAmount);
	}

	return retval;
}

FReplicationResourceMap AAbilityCombatCommander::GetAllHeldResources() const
{
	return HeldResources;
}

uint32 AAbilityCombatCommander::GetCurrentWeight() const
{
	return CurrentWeight;
}

uint32 AAbilityCombatCommander::GetMaxWeight() const
{
	return MaxWeight;
}

void AAbilityCombatCommander::GrantExp(uint32 inexp)
{
	const APlayerController * pc =  GetController<APlayerController>();
	APlayerState* ps = pc->GetPlayerState<APlayerState>();
	IExpAccumulatorInterface* expstate = Cast<IExpAccumulatorInterface>(ps);
	expstate->GrantExp(inexp);

}

void AAbilityCombatCommander::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AAbilityCombatCommander, MaxWeight);
	DOREPLIFETIME(AAbilityCombatCommander, CurrentWeight);
}
