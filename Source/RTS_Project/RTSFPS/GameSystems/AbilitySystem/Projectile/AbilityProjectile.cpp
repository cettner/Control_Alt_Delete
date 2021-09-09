// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityProjectile.h"

// Sets default values
AAbilityProjectile::AAbilityProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	
	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMoveComp"));
	MovementComp->ProjectileGravityScale = 0.0f;

	FlightParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComp"));
	CollisionComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CollisionComp"));

	RootComponent = CollisionComp;
	FlightParticles->SetupAttachment(RootComponent);

}

void AAbilityProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	FVector fv = GetActorForwardVector();
	check(MovementComp);

	MovementComp->Velocity = InitialSpeed * fv;
	SetLifeSpan(ProjectileLifeTime);
}

// Called every frame
void AAbilityProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


