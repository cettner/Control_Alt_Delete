// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RTS_Project/RTSFPS/GameSystems/UpgradeSystem/Upgrade.h"
#include "RTS_Project/RTSFPS/BaseClasses/Interfaces/RTSObjectInterface.h"
#include "RTSUpgrade.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class RTS_PROJECT_API URTSUpgrade : public UUpgrade, public IRTSObjectInterface
{
	GENERATED_BODY()

public:
	virtual bool CanUpgrade(const IUpgradableInterface * TestUpgrade) const override;
	bool IsGlobal() const;
	bool IsPersistent() const;

protected:
	UPROPERTY(EditDefaultsOnly, meta = (MustImplement = "RTSObjectInterface"))
	TArray<TSubclassOf<AActor>> TargetClasses;

	UPROPERTY(EditDefaultsOnly)
	bool bPersistsThroughRespawn = true;

	UPROPERTY(EditDefaultsOnly)
	bool bIsGlobal = true;

};
