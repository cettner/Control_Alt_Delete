// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Interface.h"
#include "Containers/Map.h"

#include "..\Upgrade.h"
#include "UpgradableInterface.generated.h"


// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UUpgradableInterface : public UInterface
{
	GENERATED_BODY()
};


class RTS_PROJECT_API IUpgradableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	/*Get CurrentTier for the specified class 0 means the Upgrade is unknown*/
	virtual uint32 GetCurrentUpgradeRankFor(const TSubclassOf<UUpgrade>& UpgradeClass) const;
	virtual TArray<TSubclassOf<UUpgrade>> GetKnownUpgrades() const;
	virtual TArray<TSubclassOf<UUpgrade>> GetUnknownUpgrades() const;
	virtual TArray<TSubclassOf<UUpgrade>> GetAllUpgrades() const;
	/**/
	virtual bool CanSupportUpgrade(const TSubclassOf<UUpgrade>& UpgradeClass);

	/*If the Actor is spawned via spawn Actor Deffered, then certain components may not be ready to be upgraded, Allows the Use of Add Upgrade and PostInstallUpgrades*/
	virtual bool CanReceiveUpgrades() const;

	virtual void OnApplyUpgrade(const UUpgrade * Upgrade);

	/*Returns the Actual Interface object to be modifed, in case it's nested in a wrapper class like a playerstate or higher level pawn. Returns the interface object by default*/
	virtual const UObject * GetUpgradeApplicationObject() const;

	virtual UObject * GetUpgradeApplicationObject();

	virtual UClass * GetUpgradeApplicationClass() const;

	virtual bool AddUpgrade(TSubclassOf<UUpgrade> UpgradeToAdd);

protected:
	virtual void PostInstallUpgrades();

};
