// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

#include "AbilityProjectile.generated.h"

UCLASS()
class RTS_PROJECT_API AAbilityProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAbilityProjectile();

protected:

	virtual void PostInitializeComponents() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;



public:
	UPROPERTY(EditDefaultsOnly)
	float InitialSpeed = 1000.0f;

	UPROPERTY(EditDefaultsOnly)
	float ProjectileLifeTime = 10.0f;

protected:
	UPROPERTY(EditDefaultsOnly)
	UProjectileMovementComponent* MovementComp = nullptr;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* CollisionComp = nullptr;

	UPROPERTY(EditAnywhere)
	UParticleSystemComponent * FlightParticles = nullptr;

};
