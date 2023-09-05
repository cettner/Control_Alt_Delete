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

void AAbilityCombatCommander::AddResource(TSubclassOf<UResource> InResourceClass, int InAmount)
{
	ResourceComp->AddResource(InResourceClass, InAmount);
}

bool AAbilityCombatCommander::RemoveResource(TSubclassOf<UResource> InResourceClass, int InAmount)
{
	return ResourceComp->RemoveResource(InResourceClass, InAmount);
}

FReplicationResourceMap AAbilityCombatCommander::GetAllHeldResources() const
{
	return ResourceComp->GetAllHeldResources();
}

uint32 AAbilityCombatCommander::GetCurrentWeight() const
{
	return ResourceComp->GetCurrentWeight();
}

uint32 AAbilityCombatCommander::GetMaxWeight() const
{
	return ResourceComp->GetMaxWeight();
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
}
