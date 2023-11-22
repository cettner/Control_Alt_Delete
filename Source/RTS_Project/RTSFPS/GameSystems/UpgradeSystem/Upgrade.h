// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Upgrade.generated.h"

constexpr uint32 UPGRADE_UNLEARNED = 0U;

/*Non Essential Helper Class for Implementing Rank System*/
USTRUCT()
struct FUpgradeInfo
{
	GENERATED_USTRUCT_BODY()
public:
	/*Class Of The Upgrade*/
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUpgrade> UpgradeClass = nullptr;

	/*Number of Times the Upgrade Has Been Applied*/
	UPROPERTY(EditDefaultsOnly)
	uint32 Rank = UPGRADE_UNLEARNED;

	friend bool operator == (const FUpgradeInfo& Myself, const FUpgradeInfo& Other)
	{
		const bool isSame = Myself.UpgradeClass == Other.UpgradeClass && Myself.Rank == Other.Rank;
		return(isSame);
	}

	friend bool operator == (const FUpgradeInfo& Myself, const TSubclassOf<UUpgrade>& Other)
	{
		const bool isSame = Myself.UpgradeClass == Other;
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


UCLASS(Abstract, Blueprintable)
class RTS_PROJECT_API UUpgrade : public UObject
{
	GENERATED_BODY()

public:
	/*Refrain From using this directly, use IUpgradableInterface::OnApplyUpgrade*/
	virtual bool ApplyUpgrade(UObject * ToUpgrade, const uint32 OldRank, const uint32 NewRank, const bool HasAuthority, const bool IsLocal) const;

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
