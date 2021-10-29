// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/SphereComponent.h"

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
	virtual void EndPlay( const EEndPlayReason::Type EndPlayReason) override;
	virtual void OnRep_Owner() override;
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const override;

public:
	void SetIgnoredActors(TArray<AActor*> IgnoreThese);
	void SetIgnoredActor(AActor * IgnoreThis);

protected:
	UFUNCTION()
	virtual void OnProjectileImpact(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
	UFUNCTION()
	virtual void OnRep_InitialSpeed();

public:
	UPROPERTY(EditDefaultsOnly)
	float DirectDamage = 40.0f;

	UPROPERTY(ReplicatedUsing = OnRep_InitialSpeed)
	float InitialSpeed = 0.0f;

	UPROPERTY(EditDefaultsOnly)
	float ProjectileLifeTime = 4.0f;

	UPROPERTY(EditDefaultsOnly)
	TEnumAsByte<ECollisionChannel> CollisionChannel;

protected:
	UPROPERTY(EditDefaultsOnly)
	UProjectileMovementComponent* MovementComp = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USphereComponent* SphereComponent = nullptr;

	UPROPERTY(EditAnywhere)
	UParticleSystemComponent * FlightParticles = nullptr;

	UPROPERTY(EditAnyWhere)
	UParticleSystem * OnHitParticles = nullptr;


protected:
	TArray<AActor *> IgnoredActors = TArray<AActor *>();

};
