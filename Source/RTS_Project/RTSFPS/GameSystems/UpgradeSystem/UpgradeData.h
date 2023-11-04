// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

#include "Upgrade.h"
#include "UpgradeData.generated.h"


UCLASS(Blueprintable, DefaultToInstanced)
class RTS_PROJECT_API UUpgradeData : public UObject
{
	GENERATED_BODY()

public:
	FORCEINLINE const TArray<FUpgradeInfo>& GetInitialUpgradeState() const { return UpgradeState; }

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
