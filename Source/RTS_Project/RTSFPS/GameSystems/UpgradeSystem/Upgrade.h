// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Upgrade.generated.h"


class IUpgradableInterface;

typedef  TPair<TSubclassOf<UUpgrade>, int > UpgradeTier_t;

UCLASS()
class RTS_PROJECT_API UUpgrade : public UObject
{
	GENERATED_BODY()

public:
	virtual void ApplyUpgrade(IUpgradableInterface * ToUpgrade) const;
	virtual bool CanUpgrade(IUpgradableInterface * TestUpgrade) const;
	uint32 GetMaxRank() const;
	FText GetToolTipInfo(uint32 CurrentRank = 0U);



	/*Configuration Data*/
protected:
	UPROPERTY(EditDefaultsOnly)
	FText TooltipInfo = FText::FromString("N/A");

	UPROPERTY(EditDefaultsOnly)
	uint32 MaxRank = 1U;
};
