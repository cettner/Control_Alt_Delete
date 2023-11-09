// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "Upgrade.h"
#include "Interfaces/UpgradableInterface.h"
#include "UpgradeData.generated.h"


DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnUpgradeChangedDelegate, const TSubclassOf<UUpgrade>/*Upgrade*/, const uint32 /*Oldrank*/, const uint32 /*NewRank*/);

UCLASS(Blueprintable)
class RTS_PROJECT_API UUpgradeData : public UObject, public IUpgradableInterface
{
	GENERATED_BODY()

public:
	FORCEINLINE const TArray<FUpgradeInfo>& GetUpgradeState() const { return UpgradeState; }
	
	virtual TArray<TSubclassOf<UUpgrade>> GetAllUpgrades() const;
	virtual uint32 GetCurrentUpgradeRankFor(const TSubclassOf<UUpgrade>& UpgradeClass) const;
	virtual bool IncrementUpgrade(const TSubclassOf<UUpgrade>& UpgradeToUnLearn);
	virtual bool DecrementUpgrade(const TSubclassOf<UUpgrade>&UpgradeToUnLearn);

	FOnUpgradeChangedDelegate OnUpgradeChanged = FOnUpgradeChangedDelegate();

protected:
	UFUNCTION()
	virtual void OnRep_UpgradeState(TArray<FUpgradeInfo> OldUpgrades);

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const;
	virtual bool IsSupportedForNetworking() const override;

protected:
	UPROPERTY(EditDefaultsOnly, ReplicatedUsing= OnRep_UpgradeState)
	TArray<FUpgradeInfo> UpgradeState = TArray<FUpgradeInfo>();
	
};
