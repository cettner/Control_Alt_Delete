// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Interface.h"

#include "..\Upgrade.h"
#include "UpgradableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UUpgradableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class RTS_PROJECT_API IUpgradableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual int GetCurrentUpgradeTierFor(TSubclassOf<UUpgrade> UpgradeClass);
	virtual int GetMaxUpgradeTierFor(TSubclassOf<UUpgrade> UpgradeClass);
	virtual TArray<TSubclassOf<UUpgrade>> GetAppliedUpgrades();

	/*If the Actor is spawned via spawn Actor Deffered, then certain components may not be ready to be upgraded, Allows the Use of Add Upgrade and PostInstallUpgrades*/
	virtual bool IsPreInitialized() const;

	virtual void OnApplyUpgrade(const UUpgrade * Upgrade);

protected:
	virtual void PostInstallUpgrades();
	virtual bool AddUpgrade(TSubclassOf<UUpgrade> UpgradeToAdd);
};
