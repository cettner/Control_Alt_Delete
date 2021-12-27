// Fill out your copyright notice in the Description page of Project Settings.


#include "ExpAccumulatorInterface.h"

// Add default functionality here for any IExpAccumulatorInterface functions that are not pure virtual.



uint32 IExpAccumulatorInterface::GetCurrentExp() const
{
	return 0U;
}

uint32 IExpAccumulatorInterface::GetMaxExpForCurrentLevel() const
{
	return 0U;
}

bool IExpAccumulatorInterface::CanRecieveExp() const
{
	const bool retval = GetCurrentLevel() < GetMaxLevel();
	return retval;
}

uint32 IExpAccumulatorInterface::GetCurrentLevel() const
{
	return 0U;
}

uint32 IExpAccumulatorInterface::GetMaxLevel() const
{
	return 0U;
}

uint32 IExpAccumulatorInterface::GetExptoNextLevel() const
{
	return 0U;
}

void IExpAccumulatorInterface::GrantExp(uint32 inexp)
{
}

void IExpAccumulatorInterface::OnLevelUp()
{
}
