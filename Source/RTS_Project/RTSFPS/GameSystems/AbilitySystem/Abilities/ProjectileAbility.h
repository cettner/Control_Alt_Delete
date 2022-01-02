// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Ability.h"
#include "../Projectile/AbilityProjectile.h"
#include "ProjectileAbility.generated.h"

/**
 * 
 */
UCLASS()
class RTS_PROJECT_API UProjectileAbility : public UAbility
{
	GENERATED_BODY()



public:
	virtual void OnEffect() override;
	virtual void SetProjectileSpeed(float NewSpeed);
	virtual float GetProjectileSpeed() const;

	virtual void SetProjectileDamage(float NewDamage);
	virtual float GetProjectileDamage() const;

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AAbilityProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly)
	float ProjectileSpeed = 1000.0f;

	UPROPERTY(EditDefaultsOnly)
	float ProjectileDamage = 40.0f;
};
