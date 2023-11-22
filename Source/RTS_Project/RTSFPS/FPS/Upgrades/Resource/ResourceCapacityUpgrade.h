// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "../../../GameSystems/ResourceSystem/Resource.h"
#include "ResourceUpgrade.h"
#include "ResourceCapacityUpgrade.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UResourceCapacityUpgrade : public UResourceUpgrade
{
	GENERATED_BODY()
		
public:
	virtual bool ApplyUpgrade(UObject* ToUpgrade, const uint32 OldRank, const uint32 NewRank, const bool HasAuthority, const bool IsLocal) const override;


protected:
	UPROPERTY(EditDefaultsOnly)
	bool UpdateWeight = false;

	UPROPERTY(EditDefaultsOnly, meta=(EditCondition = "UpdateWeight == false"))
	TSubclassOf<UResource> TargetResource = nullptr;

	UPROPERTY(EditDefaultsOnly)
	uint32 BaseChangeValue = 0U;
};
