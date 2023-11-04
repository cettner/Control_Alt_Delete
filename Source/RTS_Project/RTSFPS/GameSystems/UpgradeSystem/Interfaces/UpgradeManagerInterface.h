// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"

#include "UpgradableInterface.h"
#include "UpgradeManagerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UUpgradeManagerInterface : public UInterface
{
	GENERATED_BODY()
};


class RTS_PROJECT_API IUpgradeManagerInterface
{
	GENERATED_BODY()

public:
	virtual bool HasAuthority();

public:
	virtual void AddUpgrade(const TSubclassOf<UUpgrade>& InUpgrade);
	virtual void RemoveUpgrade(const TSubclassOf<UUpgrade>& InUpgrade);

public:
	virtual bool RegisterUpgradable(IUpgradableInterface * InUpgradeObject);
	virtual bool UnRegisterUpgradable(IUpgradableInterface * InUpgradeObject);

	virtual const TArray<const TSubclassOf<UUpgrade>> GetAllUpgradesFor(IUpgradableInterface* InUpgradeObject) const;
	virtual const TArray<TScriptInterface<IUpgradableInterface>> GetUpgradeSubScribers() const;

protected:
	virtual const TArray<const TSubclassOf<UUpgrade>> GetAllAppliedUpgrades() const;


};
