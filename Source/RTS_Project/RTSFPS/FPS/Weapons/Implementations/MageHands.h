// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../AbilityWeapon.h"
#include "MageHands.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API AMageHands : public AAbilityWeapon
{
	GENERATED_BODY()
	
protected:
	virtual USkeletalMeshComponent* GetWeaponMesh() const override;
};
