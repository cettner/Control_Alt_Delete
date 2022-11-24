// Fill out your copyright notice in the Description page of Project Settings.


#include "CombatInterface.h"


bool ICombatInterface::IsAttacking() const
{
	return false;
}


bool ICombatInterface::StartAttack(const int32 InAttackID)
{
	return false;
}

FHitResult ICombatInterface::PerformTrace()
{
	FHitResult retval = FHitResult();
	return retval;
}

int32 ICombatInterface::GetAttackIndexForTarget(const AActor* InToAttack) const
{
	return CANT_ATTACK_INDEX;
}