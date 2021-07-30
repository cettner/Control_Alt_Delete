// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityComponent.h"

// Sets default values for this component's properties
UAbilityComponent::UAbilityComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

TWeakObjectPtr<UAbility> UAbilityComponent::GetCurrentAbility()
{
	return TWeakObjectPtr<UAbility>();
}

bool UAbilityComponent::IsAbilityUsingCrosshair()
{
	return false;
}

bool UAbilityComponent::IsUsingAbility()
{
	return false;
}

float UAbilityComponent::GetManaCost()
{
	return 0.0f;
}

bool UAbilityComponent::GetIsCasting()
{
	return false;
}

bool UAbilityComponent::IsAbilityValid()
{
	bool retval = IsValid(CurrentAbility);
	return retval;
}

FVector UAbilityComponent::GetControlRotation()
{
	return FVector();
}

FTransform UAbilityComponent::GetSurfaceTransform()
{
	return FTransform();
}
