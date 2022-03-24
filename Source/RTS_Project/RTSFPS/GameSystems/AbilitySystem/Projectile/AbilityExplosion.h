// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"

#include "AbilityExplosion.generated.h"

UCLASS()
class RTS_PROJECT_API AAbilityExplosion : public AActor
{
	GENERATED_BODY()
	
public:	
	AAbilityExplosion();

protected:
	virtual void PostInitializeComponents() override;

public:
	UPROPERTY(EditDefaultsOnly)
	float DirectDamage = 40.0f;

	UPROPERTY(EditDefaultsOnly)
	float ProjectileLifeTime = 4.0f;

	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<ECollisionChannel> CollisionChannel;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* SphereComponent = nullptr;

	UPROPERTY(EditAnyWhere)
	UParticleSystem * BaseParticleEffect = nullptr;

protected:
	TArray<AActor *> IgnoredActors = TArray<AActor *>();

};
