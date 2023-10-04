// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "Resource.h"
#include "ResourceVendorData.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UResourceVendorData : public UDataAsset
{
	GENERATED_BODY()

	public:
		const TMap<TSubclassOf<UObject>, FReplicationResourceMap>& GetDefaultPriceMap() const { return DefaultPriceMap; }

	protected:
		UPROPERTY(EditDefaultsOnly)
		TMap<TSubclassOf<UObject>, FReplicationResourceMap> DefaultPriceMap = TMap<TSubclassOf<UObject>, FReplicationResourceMap>();

};
