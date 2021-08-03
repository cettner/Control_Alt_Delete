// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityUserInterface.h"

// Add default functionality here for any IAbilityUserInterface functions that are not pure virtual.

bool IAbilityUserInterface::CanCastAbility()
{
	return (false);
}

float IAbilityUserInterface::PlayAbilityMontage(FAbilityAnim AnimToPlay)
{
	return 0.0f;
}
