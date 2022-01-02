// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Upgrade.generated.h"


class IUpgradableInterface;


USTRUCT()
struct FUpgradeUnlockCondition
{
	GENERATED_USTRUCT_BODY()
public:
	TSubclassOf<UUpgrade> GetParent() const { return Parent; }

	uint32 GetRank() const { return Rank; }

private:
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UUpgrade> Parent = nullptr;

	UPROPERTY(EditDefaultsOnly)
		uint32 Rank = 1U;
};


UCLASS()
class RTS_PROJECT_API UUpgrade : public UObject
{
	GENERATED_BODY()

public:
	/*Refrain From using this directly, use IUpgradableInterface::OnApplyUpgrade*/
	virtual void ApplyUpgrade(UObject * ToUpgrade) const;
	virtual bool CanUpgrade(const IUpgradableInterface * TestUpgrade) const;
	
	uint32 GetMaxRank() const;
	FText GetToolTipInfo(uint32 CurrentRank = 0U)  const;
	TArray<TSubclassOf<UUpgrade>> GetExclusiveConditions() const;
	TArray<FUpgradeUnlockCondition> GetUnlockConditions() const;

	/*Configuration Data*/
protected:
	UPROPERTY(EditDefaultsOnly)
	FText UpgradeName = FText::FromString("DefaultUpgrade Name");

	UPROPERTY(EditDefaultsOnly)
	FText TooltipInfo = FText::FromString("Long Long Long Long Text");

	UPROPERTY(EditDefaultsOnly)
	uint32 MaxRank = 1U;

	UPROPERTY(EditDefaultsOnly)
	TArray<FUpgradeUnlockCondition> UnlockConditions = TArray<FUpgradeUnlockCondition>();

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UUpgrade>> ExclusiveConditions = TArray<TSubclassOf<UUpgrade>>();
};
