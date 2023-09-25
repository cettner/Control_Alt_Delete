// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "Resource.h"
#include "ResourceData.generated.h"

/**
 * 
 */

USTRUCT()
struct FResourceConfigData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly)
	uint32 Max = 0xFFFFFFFF;

	UPROPERTY(EditDefaultsOnly)
	uint32 Min = 0U;

	UPROPERTY(EditDefaultsOnly)
	uint32 StartingValue = 0U;
};

UCLASS()
class RTS_PROJECT_API UResourceData : public UDataAsset
{
	GENERATED_BODY()

public:
	const FReplicationResourceMap& GetResourceData() const { return Resources; }
	const TMap<TSubclassOf<UResource>, FResourceConfigData>& GetResourceConfig() const { return ResourceConfig; }
protected:
	UPROPERTY(EditDefaultsOnly)
	FReplicationResourceMap Resources = FReplicationResourceMap();

	UPROPERTY(EditDefaultsOnly)
	TMap<TSubclassOf<UResource>, FResourceConfigData> ResourceConfig = TMap<TSubclassOf<UResource>, FResourceConfigData>();
};
