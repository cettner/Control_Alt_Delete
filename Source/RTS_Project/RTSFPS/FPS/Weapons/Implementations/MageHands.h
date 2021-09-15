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
	AMageHands();

protected:
	virtual USkeletalMeshComponent* GetWeaponMesh() const override;
	virtual void InitAbilities() override;


	virtual void StartSecondaryFire() override;

	virtual void StopSecondaryFire() override;

protected:
	/*Container For All Right Hand Assets*/
	UAbilityComponent * RightHandAbilityComp = nullptr;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UAbility>> RightAbilityClasses;
};
