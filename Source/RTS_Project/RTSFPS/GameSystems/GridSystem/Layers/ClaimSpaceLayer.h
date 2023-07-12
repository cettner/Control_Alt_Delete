// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "../GridLayer.h"
#include "ClaimSpaceLayer.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UClaimSpaceLayer : public UGridLayer
{
	GENERATED_BODY()

	UClaimSpaceLayer();

	public:
		bool IsClaimValid() const;

	protected:
		virtual void ShowTile(UGridTile* InTile) override;
		virtual void HideTile(UGridTile* InTile) override;
};
