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

uint32 IExpAccumulatorInterface::GetExpforLevel(uint32 inLevel) const
{
	return 0U;
}

int32 IExpAccumulatorInterface::GetExptoNextLevel() const
{
	return 0;
}

uint32 IExpAccumulatorInterface::GetAvailableUpgradePoints() const
{
	return uint32();
}

uint32 IExpAccumulatorInterface::GetSpentUpgradePoints() const
{
	return uint32();
}

uint32 IExpAccumulatorInterface::GetTotalUpgradePoints() const
{
	return uint32();
}

bool IExpAccumulatorInterface::SpendUpgradePoints(uint32 PointsToSpend)
{
	return false;
}

void IExpAccumulatorInterface::GrantExp(uint32 inexp)
{
}

void IExpAccumulatorInterface::OnLevelUp()
{
}
