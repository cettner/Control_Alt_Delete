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
	virtual bool InstallUpgrade(const TSubclassOf<UUpgrade>& InUpgrade, const uint32 InRank);
	/*Installs the upgrade if unknown, will attempt to increase rank if it is already known*/
	virtual bool InstallUpgrade(const TSubclassOf<UUpgrade>& InUpgrade);
	/*Set the rank of the upgrade to UNKNOWN_UPGRADE equivalent of InstallUpgrade(InUpgrade,UNKNOWN_UPGRADE)*/
	virtual bool UninstallUpgrade(const TSubclassOf<UUpgrade>& InUpgrade);
	/*Set the rank of the upgrade to it's maximum rank*/
	virtual bool MaxInstallUpgrade(const TSubclassOf<UUpgrade>& InUpgrade);
	/*Get CurrentTier for the specified class 0 means the Upgrade is unknown*/
	virtual uint32 GetCurrentUpgradeRankFor(const TSubclassOf<UUpgrade>& UpgradeClass) const;
	virtual TArray<TSubclassOf<UUpgrade>> GetKnownUpgrades() const;
	virtual TArray<TSubclassOf<UUpgrade>> GetUnknownUpgrades() const;
	virtual TArray<TSubclassOf<UUpgrade>> GetAllUpgrades() const;
	virtual bool MeetsUpgradeDependencies(const TSubclassOf<UUpgrade>& UpgradeClass) const;
	virtual bool MeetsUpgradeDependencies(const TSubclassOf<UUpgrade>& UpgradeClass, TArray<FUpgradeDependencyInfo>& OutDependencyInfo) const;

	virtual TArray<TSubclassOf<UUpgrade>> GetInstalledUpgrades() const;
	/**/
	virtual bool CanSupportUpgrade(const TSubclassOf<UUpgrade>& UpgradeClass) const;

protected:
	virtual bool LearnUpgrade(const TSubclassOf<UUpgrade>& UpgradeToAdd);
	virtual bool UnLearnUpgrade(const TSubclassOf<UUpgrade>& UpgradeToUnLearn);
	virtual bool IncrementUpgrade(const TSubclassOf<UUpgrade>& UpgradeToUnLearn) PURE_VIRTUAL(IUpgradableInterface::IncrementUpgrade, return false;);
	virtual bool DecrementUpgrade(const TSubclassOf<UUpgrade>& UpgradeToUnLearn) PURE_VIRTUAL(IUpgradableInterface::DecrementUpgrade, return false;);
};
