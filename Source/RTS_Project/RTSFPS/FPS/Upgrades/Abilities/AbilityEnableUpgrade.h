// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityUserUpgrade.h"
#include "AbilityEnableUpgrade.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UAbilityEnableUpgrade : public UAbilityUserUpgrade
{
	GENERATED_BODY()

	UAbilityEnableUpgrade();

public:
	virtual bool ApplyUpgrade(UObject* ToUpgrade, const uint32 OldRank, const uint32 NewRank, const bool HasAuthority, const bool IsLocal) const override;

protected:
	UPROPERTY(EditDefaultsOnly)
	TSet<TSubclassOf<UAbility>> TargetAbilites = TSet<TSubclassOf<UAbility>>();
};
