// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ResourceUpgrade.h"

#include "../../../GameSystems/ResourceSystem/Interfaces/ResourceGatherer.h"
#include "ResourceRegenUpgrade.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UResourceRegenUpgrade : public UResourceUpgrade
{
	GENERATED_BODY()
	
public:
	virtual bool ApplyUpgrade(UObject* ToUpgrade, const uint32 OldRank, const uint32 NewRank, const bool HasAuthority, const bool IsLocal) const override;
	virtual FResourceRegenEventConfig BuildConfigFromRank(const IResourceGatherer* InGatherer, const uint32 OldRank, const uint32 NewRank, bool& OutShouldClearEvent) const;

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UResource> TargetResource = nullptr;

	UPROPERTY(EditDefaultsOnly)
	uint32 GainBasePerRank = 0U;

	UPROPERTY(EditDefaultsOnly)
	float RateBasePerRank = 0U;

	UPROPERTY(EditDefaultsOnly)
	/*Used if no existing RegenEvent is found upon Application*/
	FResourceRegenEventConfig DefaultRegenConfig = FResourceRegenEventConfig();
};
