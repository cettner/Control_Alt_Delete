// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Upgrade.generated.h"


class IUpgradableInterface;

constexpr uint32 UPGRADE_UNLEARNED = 0U;

/*Non Essential Helper Class for Implementing Rank System*/
USTRUCT()
struct FUpgradeInfo
{
	GENERATED_USTRUCT_BODY()
public:
	/*Class Of The Upgrade*/
	UPROPERTY(EditDefaultsOnly, NotReplicated)
	TSubclassOf<UUpgrade> UpgradeClass = nullptr;

	/*Number of Times the Upgrade Has Been Applied*/
	UPROPERTY(EditDefaultsOnly)
	int Rank = UPGRADE_UNLEARNED;

	/*For Comparision, We only care that the class is the same,  That way, if we want to "Find" it in an array
	we can simulate a "MultiMap" Style Behavior Between the upgrade and its Rank*/
	friend bool operator == (const FUpgradeInfo& Myself, const FUpgradeInfo& Other)
	{
		bool isSame = Myself.UpgradeClass == Other.UpgradeClass;
		return(isSame);
	}
};

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

enum EUpgradeDependencyCategory
{
	UNDEFINED,
	UNLOCK,
	EXCLUSION,
	PURCHASE_PRICE
};

struct FUpgradeDependencyInfo
{
public:
	FString Description = "Requires X Points In Y";

	bool IsSatisfied = false;

	EUpgradeDependencyCategory Category = UNDEFINED;
};


UCLASS()
class RTS_PROJECT_API UUpgrade : public UObject
{
	GENERATED_BODY()

public:
	/*Refrain From using this directly, use IUpgradableInterface::OnApplyUpgrade*/
	virtual void ApplyUpgrade(UObject * ToUpgrade) const;
	virtual bool CanUpgrade(const IUpgradableInterface * TestUpgrade) const;
	virtual bool CanUpgrade(const IUpgradableInterface* TestUpgrade, TArray<FUpgradeDependencyInfo>& OutDependencyInfo) const;

	uint32 GetMaxRank() const;
	FString GetUpgradeDescription(uint32 CurrentRank = 0U)  const;
	FName GetUpgradeName() const;

	TArray<TSubclassOf<UUpgrade>> GetExclusiveConditions() const;
	TArray<FUpgradeUnlockCondition> GetUnlockConditions() const;

	/*Configuration Data*/
protected:
	UPROPERTY(EditDefaultsOnly)
	FName UpgradeName = "DefaultUpgrade Name";

	UPROPERTY(EditDefaultsOnly)
	FString TooltipInfo = "Long Long Long Long Text";

	UPROPERTY(EditDefaultsOnly)
	uint32 MaxRank = 1U;

	UPROPERTY(EditDefaultsOnly)
	TArray<FUpgradeUnlockCondition> UnlockConditions = TArray<FUpgradeUnlockCondition>();

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UUpgrade>> ExclusiveConditions = TArray<TSubclassOf<UUpgrade>>();
};
