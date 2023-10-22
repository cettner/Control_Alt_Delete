// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "Resource.h"
#include "ResourceData.generated.h"

/**
 * 
 */
DECLARE_DELEGATE_RetVal(int32, FGetResource);


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

USTRUCT()
struct FResourceRegenEventConfig
{
	GENERATED_USTRUCT_BODY()

	friend class UResourceGathererComponent;

public:
	int32 GetRegenAmount() const
	{
		int32 retval = FlatRegenAmount;
		if (RegenAmountHandler.IsBound())
		{
			retval = RegenAmountHandler.Execute();
		}

		return retval;
	}

public:
	UPROPERTY(EditDefaultsOnly)
	float TickRate = .5f;

	/*Regen amount per tick*/
	UPROPERTY(EditDefaultsOnly)
	int32 FlatRegenAmount = 0;

	UPROPERTY(EditDefaultsOnly)
	/*Whether the regen should start immediatly, this might disabled if you want to configure RegenAmountHandler before play start*/
	bool bEnabledAtStart = true;

	/*Optionally allows for an external source to supply the amount of regen provided per tick*/
	FGetResource RegenAmountHandler = FGetResource();

protected:
	/*Runtime Specifies the function to execute when the timer elapses*/
	FTimerDelegate TimerDelegate = FTimerDelegate();

	/*Allows cancellation of the regen timer*/
	FTimerHandle TimerHandle = FTimerHandle();

	TSubclassOf<UResource> ResourceClass = nullptr;
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
