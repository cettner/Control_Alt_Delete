// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "Interfaces/ResourceGatherer.h"
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
	const TMap<TSubclassOf<UResource>, FResourceConfigData>& GetResourceConfig() const { return ResourceConfig; }

	const TMap<TSubclassOf<UResource>, FResourceRegenEventConfig>& GetReSourceRegenConfig() const { return ResourceRegenConfig; }

protected:
	UPROPERTY(EditDefaultsOnly)
	TMap<TSubclassOf<UResource>, FResourceConfigData> ResourceConfig = TMap<TSubclassOf<UResource>, FResourceConfigData>();

	UPROPERTY(EditDefaultsOnly)
	TMap<TSubclassOf<UResource>, FResourceRegenEventConfig> ResourceRegenConfig = TMap<TSubclassOf<UResource>, FResourceRegenEventConfig>();
};
