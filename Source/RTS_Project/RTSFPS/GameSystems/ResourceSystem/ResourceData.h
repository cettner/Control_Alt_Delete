// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "Resource.h"
#include "ResourceData.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UResourceData : public UDataAsset
{
	GENERATED_BODY()

public:
	const FReplicationResourceMap& GetResourceData() const { return Resources; }

protected:
	UPROPERTY(EditDefaultsOnly)
	FReplicationResourceMap Resources = FReplicationResourceMap();
};
