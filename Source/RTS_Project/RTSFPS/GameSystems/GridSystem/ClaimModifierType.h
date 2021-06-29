// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GridModifierType.h"
#include "ClaimModifierType.generated.h"


class AClaimableSquareGameGrid;

UCLASS()
class RTS_PROJECT_API UClaimModifierType : public UGridModifierType
{
	GENERATED_BODY()
	

	protected:
		virtual void OnModifierApply(AClaimableSquareGameGrid * ParentGrid, FGridTile TileLocation, AGridClaimingActor * Invoker) override;
		virtual void OnModiferRemoved(AClaimableSquareGameGrid * ParentGrid, FGridTile TileLocation, AGridClaimingActor * Invoker) override;
};
