// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ExpAccumulatorInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UExpAccumulatorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RTS_PROJECT_API IExpAccumulatorInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual uint32 GetCurrentExp() const;
	virtual uint32 GetMaxExpForCurrentLevel() const;
	virtual bool CanRecieveExp() const;
	virtual uint32 GetCurrentLevel() const;
	virtual uint32 GetMaxLevel() const;
	virtual uint32 GetExpforLevel(uint32 inLevel) const;
	virtual int32 GetExptoNextLevel() const;

	virtual void GrantExp(uint32 inexp);

public:
	virtual uint32 GetAvailableUpgradePoints() const;
	virtual uint32 GetSpentUpgradePoints() const;
	virtual uint32 GetTotalUpgradePoints() const;

	virtual bool SpendUpgradePoints(uint32 PointsToSpend = 1U);

protected:
	virtual void OnLevelUp();
};
