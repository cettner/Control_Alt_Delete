// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityUserInterface.h"

// Add default functionality here for any IAbilityUserInterface functions that are not pure virtual.

bool IAbilityUserInterface::CanCastAbility() const
{
	return (false);
}

float IAbilityUserInterface::PlayAbilityMontage(FAbilityAnim AnimToPlay)
{
	return 0.0f;
}

void IAbilityUserInterface::StopAbilityMontage(FAbilityAnim AnimToStop)
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

TArray<TWeakObjectPtr<UAbility>> IAbilityUserInterface::GetAbilitiesByClass(TSubclassOf<UAbility> AbilityClass) const
{
	return TArray<TWeakObjectPtr<UAbility>>();
}

void IAbilityUserInterface::AddAbility(TSubclassOf<UAbility> AbilityClass, AActor* InSource, TArray<FName> InInstallTags)
{
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

USceneComponent * IAbilityUserInterface::GetParticleAttatchmentComponent(TWeakObjectPtr<UAbility> SpawningAbility)
{
	return nullptr;
}
