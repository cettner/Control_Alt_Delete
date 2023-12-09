// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityUserInterface.h"

// Add default functionality here for any IAbilityUserInterface functions that are not pure virtual.

bool IAbilityUserInterface::CanCastAbility(const UAbility * TracingAbility) const
{
	return (false);
}

bool IAbilityUserInterface::SpendAbilityCost(const UAbility * SpendingAbility)
{
	return false;
}

float IAbilityUserInterface::PlayAbilityMontage(const FAbilityAnim& AnimToPlay)
{
	return 0.0f;
}

void IAbilityUserInterface::StopAbilityMontage(const FAbilityAnim& AnimToPlay)
{

}

FVector IAbilityUserInterface::GetAbilitySocketLocation(FName SocketName) const 
{
	return FVector();
}

FVector IAbilityUserInterface::GetAbilityAimVector() const
{
	return FVector();
}

TArray<AActor*> IAbilityUserInterface::GetIgnoredTraceActors(TWeakObjectPtr<UAbility> TracingAbility)
{
	return TArray<AActor*>();
}

UAbility * IAbilityUserInterface::GetFirstAbilityByClass(const TSubclassOf<UAbility>& InAbilityClass) const
{
	return nullptr;
}

TArray<UAbility*> IAbilityUserInterface::GetAbilitiesByClass(const TSubclassOf<UAbility>& InAbilityClass) const
{
	return TArray<UAbility*>();
}

TArray<UAbility*> IAbilityUserInterface::GetAbilitiesByTag(FName InTag) const
{
	return TArray<UAbility*>();
}

void IAbilityUserInterface::EnableAbility(const TSubclassOf<UAbility>& AbilityClass)
{

}

bool IAbilityUserInterface::DisableAbility(const TSubclassOf<UAbility>& AbilityClass)
{
	return false;
}

bool IAbilityUserInterface::IsAbilityEnabled(const TSubclassOf<UAbility>& AbilityClass) const
{
	return false;
}

TSet<TSubclassOf<UAbility>> IAbilityUserInterface::GetSupportedAbilities() const
{
	return TSet<TSubclassOf<UAbility>>();
}

bool IAbilityUserInterface::SupportsAbility(const TSubclassOf<UAbility>& Inabilityclass) const
{
	return GetSupportedAbilities().Contains(Inabilityclass);
}

void IAbilityUserInterface::OnReadyNotify(UAbilityAnimNotify * CallingContext)
{
}

void IAbilityUserInterface::OnLoopNotify(UAbilityAnimNotify * CallingContext)
{
}

void IAbilityUserInterface::OnEffectNotify(UAbilityAnimNotify * CallingContext)
{
}

void IAbilityUserInterface::OnEndNotify(UAbilityAnimNotify * CallingContext)
{
}

void IAbilityUserInterface::OnTickNotify(float InFrameDeltaTime, const FAnimNotifyEventReference& InEventReference)
{
}

USceneComponent * IAbilityUserInterface::GetParticleAttatchmentComponent(TWeakObjectPtr<UAbility> SpawningAbility)
{
	return nullptr;
}
