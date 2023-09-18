// Fill out your copyright notice in the Description page of Project Settings.


#include "ConjurableInterface.h"

// Add default functionality here for any IConjurableInterface functions that are not pure virtual.


void IConjurableInterface::OnConjureStopped()
{

}

void IConjurableInterface::OnConjureStarted()
{
}

IAbilityUserInterface* IConjurableInterface::GetConjurer() const
{
	return nullptr;
}

bool IConjurableInterface::IsConjured() const
{
	return false;
}

TWeakObjectPtr<UAbility> IConjurableInterface::GetSpawnedAbility() const
{
	return TWeakObjectPtr<UAbility>();
}

TSubclassOf<UAbility> IConjurableInterface::GetSpawnAbilityClass() const
{
	return TSubclassOf<UAbility>();
}

void IConjurableInterface::ConjureInit(IAbilityUserInterface* InConjurer, UConjureAbility* InAbility)
{
}

void IConjurableInterface::OnConjureComplete()
{
}
