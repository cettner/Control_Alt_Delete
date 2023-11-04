// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/UpgradeManagerInterface.h"

#include "UpgradeManagerComponent.generated.h"


UCLASS()
class RTS_PROJECT_API UUpgradeManagerComponent : public UActorComponent, public IUpgradeManagerInterface
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UUpgradeManagerComponent();


/***********************************************************************/
public:
	virtual void AddUpgrade(const TSubclassOf<UUpgrade>& InUpgrade) override;
	virtual void RemoveUpgrade(const TSubclassOf<UUpgrade>& InUpgrade) override;

public:
	virtual bool RegisterUpgradable(IUpgradableInterface* InUpgradeObject) override;
	virtual bool UnRegisterUpgradable(IUpgradableInterface* InUpgradeObject) override;
/***********************************************************************/

protected:
	UFUNCTION()
	virtual void OnRep_AppliedUpgrades();

protected:
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

protected:
	TMultiMap<const TSubclassOf<UUpgrade>, TScriptInterface<IUpgradeManagerInterface>> ResourceMapping = TMultiMap<const TSubclassOf<UUpgrade>, TScriptInterface<IUpgradeManagerInterface>>();

	UPROPERTY(EditDefaultsOnly, ReplicatedUsing = OnRep_AppliedUpgrades)
	TArray<FUpgradeInfo> AllUpgrades = TArray<FUpgradeInfo>();
};
