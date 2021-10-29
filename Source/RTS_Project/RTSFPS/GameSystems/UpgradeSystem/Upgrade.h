// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Upgrade.generated.h"


class IUpgradableInterface;

UCLASS()
class RTS_PROJECT_API UUpgrade : public UObject
{
	GENERATED_BODY()
public:
	virtual void ApplyUpgrade(IUpgradableInterface * ToUpgrade) const;
	virtual bool CanUpgrade(IUpgradableInterface * TestUpgrade) const;

protected:
	UPROPERTY(EditDefaultsOnly)
	FString TooltipInfo = "N/A";
};
