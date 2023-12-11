// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "../../../GameSystems/UpgradeSystem/Upgrade.h"
#include "MovementUpgrade.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UMovementUpgrade : public UUpgrade
{
	GENERATED_BODY()
	

public:
	virtual bool ApplyUpgrade(UObject* ToUpgrade, const uint32 OldRank, const uint32 NewRank, const bool HasAuthority, const bool IsLocal) const override;

protected:
	UPROPERTY(EditDefaultsOnly)
	float BaseMovementEnhancment = 50.0f;
};
