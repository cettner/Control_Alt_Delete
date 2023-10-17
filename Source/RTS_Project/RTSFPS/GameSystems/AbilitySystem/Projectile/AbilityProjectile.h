// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"

#include "AbilityExplosion.h"
#include "AbilityProjectile.generated.h"

UCLASS()
class RTS_PROJECT_API AAbilityProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	AAbilityProjectile();

protected:
	virtual void PostInitializeComponents() override;

public:	
	virtual void OnRep_Owner() override;
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

public:
	void SetIgnoredActors(TArray<AActor*> IgnoreThese);
	void SetIgnoredActor(AActor * IgnoreThis);

	AAbilityExplosion* SpawnExplosionatLocation(FTransform InTransform) const;

protected:
	UFUNCTION()
	virtual void OnProjectileImpact(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	virtual void OnDetonation(const FHitResult& Hit = FHitResult());

protected:
	UFUNCTION()
	virtual void OnRep_InitialSpeed();

	UFUNCTION()
	virtual void OnRep_bHasDetonated();

public:
	UPROPERTY(EditDefaultsOnly)
	float DirectDamage = 40.0f;

	UPROPERTY(ReplicatedUsing = OnRep_InitialSpeed)
	float InitialSpeed = 0.0f;
	
	UPROPERTY(ReplicatedUsing = OnRep_bHasDetonated)
	bool bHasDetonated = false;

	UPROPERTY(EditDefaultsOnly)
	float ProjectileLifeTime = 4.0f;

	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<ECollisionChannel> CollisionChannel;

	UPROPERTY(EditDefaultsOnly)
	bool bDetonatesOnImpact = true;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AAbilityExplosion> ExplosionClass = nullptr;

protected:
	UPROPERTY(EditDefaultsOnly)
	UProjectileMovementComponent* MovementComp = nullptr;

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* SphereComponent = nullptr;

	UPROPERTY(EditDefaultsOnly)
	UParticleSystemComponent * FlightParticles = nullptr;


protected:
	TArray<AActor *> IgnoredActors = TArray<AActor *>();

};
